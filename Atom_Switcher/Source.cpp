#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>
#include "Atom.h"

using namespace std;

const long double CC = 1.9;
const long double SiC = 2.24;
const long double CH = 1.2;
const long double SiSi = 2.6;
const long double SiH = 1.6;
const long double HH = 1.0;

int main() {
	ifstream fin;
	fin.open("CONTCAR");

	//defining variables we need to read in the file
	string system_name;
	long double lattice_const;
	long double a1[3];
	long double a2[3];
	long double a3[3];
	string atoms[3];
	int amount[3];
	string coordinate_type;

	//now to read in the file
	getline(fin, system_name);
	fin >> lattice_const;
	fin >> a1[0] >> a1[1] >> a1[2];
	fin >> a2[0] >> a2[1] >> a2[2];
	fin >> a3[0] >> a3[1] >> a3[2];
	fin >> atoms[0] >> atoms[1] >> atoms[2];
	fin >> amount[0] >> amount[1] >> amount[2];
	fin >> coordinate_type;

	vector<Atom> carbon;
	vector<Atom> silicon;
	vector<Atom> hydrogen;
	vector<Atom> bonded;

	//create a dummy atom to change its data and add to the back of vectors
	Atom some;

	//reading in all the first type of atom
	char test = atoms[0][0];
	for (int i = 0; i < amount[0]; i++) {
		fin >> some.x_direct;
		fin >> some.y_direct;
		fin >> some.z_direct;
		switch (test)
		{
		case 'C':
			carbon.push_back(some);
			break;
		case 'S':
			silicon.push_back(some);
			break;
		case 'H':
			hydrogen.push_back(some);
		default:
			break;
		}
	}

	//reading in all of the second type of atom
	test = atoms[1][0];
	for (int i = 0; i < amount[1]; i++) {
		fin >> some.x_direct;
		fin >> some.y_direct;
		fin >> some.z_direct;
		switch (test)
		{
		case 'C':
			carbon.push_back(some);
			break;
		case 'S':
			silicon.push_back(some);
			break;
		case 'H':
			hydrogen.push_back(some);
		default:
			break;
		}
	}

	//reading in all of the third type of atom
	test = atoms[2][0];
	for (int i = 0; i < amount[2]; i++) {
		fin >> some.x_direct;
		fin >> some.y_direct;
		fin >> some.z_direct;
		switch (test)
		{
		case 'C':
			carbon.push_back(some);
			break;
		case 'S':
			silicon.push_back(some);
			break;
		case 'H':
			hydrogen.push_back(some);
		default:
			break;
		}
	}

	//now to get all the cartesian coordinates for the atoms
	for (unsigned i = 0; i < carbon.size(); i++)
		carbon[i].set_cartesian(a1, a2, a3);
	for (unsigned i = 0; i < silicon.size(); i++)
		silicon[i].set_cartesian(a1, a2, a3);
	for (unsigned i = 0; i < hydrogen.size(); i++)
		hydrogen[i].set_cartesian(a1, a2, a3);

	long double distance = 0;
	//figuring out the silicon atoms that are bonded to hydrogens
	//this does not get all of the bonds I've found out
	//but it gets most and is good enough for my purposes...
	for (unsigned i = 0; i < silicon.size(); i++) {
		for (unsigned j = 0; j < hydrogen.size(); j++) {
			distance = find_distance(silicon[i], hydrogen[j]);
			if (distance <= SiH)
				silicon[i].h_bonded = true;
		}
	}

	//figuring out the carbon atoms that are bonded to hydrogens
	//this also does not get ALL of the SiH bonds, but for my purposes is good enough
	for (unsigned i = 0; i < carbon.size(); i++) {
		for (unsigned j = 0; j < hydrogen.size(); j++) {
			distance = find_distance(carbon[i], hydrogen[j]);
			if (distance <= CH)
				carbon[i].h_bonded = true;
		}
	}

	//figuring out all of the silicon and carbon atoms that are bonded to each other
	for (unsigned i = 0; i < silicon.size(); i++) {
		for (unsigned j = 0; j < carbon.size(); j++) {
			distance = find_distance(silicon[i], carbon[j]);
			if (distance <= SiC) {
				silicon[i].c_bonded = true;
				carbon[j].si_bonded = true;
			}
		}
	}


	//what would the user like this program to execute specifically?
	int lever = 0, desired_atoms = 0;
	cout << "Would you like to..." << endl;
	cout << "(1) Switch C-H to Si-H," << endl;
	cout << "(2) Switch Si-H to C-H," << endl;
	cout << "(3) Switch Si-C to Si-Si," << endl;
	cout << "(4) Switch Si-C to C-C?" << endl;
	cin >> lever;
	int random;
	int a, b;

	switch (lever) {
	case 1:
		//case for when the user wants to add more SiSi bonds to the model
		for (unsigned i = 0; i < carbon.size(); i++) {
			if (carbon[i].h_bonded == true) {
				bonded.push_back(carbon[i]);
				carbon.erase(carbon.begin() + i);
			}
		}

		cout << "How many of the " << bonded.size() << " bonded Carbons would you like to replace?" << endl;
		cin >> desired_atoms;

		srand(time(NULL));

		for (int i = 0; i < desired_atoms; i++) {
			random = rand() % bonded.size();
			some = bonded[random];
			bonded.erase(bonded.begin() + random);
			silicon.push_back(some);
		}

		for (unsigned i = 0; i < bonded.size(); i++) {
			carbon.push_back(bonded[i]);
		}

		a = bonded.size();
		for (int i = 0; i < a; i++) {
			bonded.erase(bonded.begin());
		}

		break;
	case 2:
		//case for when the user wants to add more CC bonds to the model
		for (unsigned i = 0; i < silicon.size(); i++) {
			if (silicon[i].h_bonded == true) {
				bonded.push_back(silicon[i]);
				silicon.erase(silicon.begin() + i);
			}
		}

		cout << "How many of the " << bonded.size() << " bonded Silicons would you like to replace?" << endl;
		cin >> desired_atoms;

		srand(time(NULL));

		for (int i = 0; i < desired_atoms; i++) {
			random = rand() % bonded.size();
			some = bonded[random];
			bonded.erase(bonded.begin() + random);
			carbon.push_back(some);
		}

		for (unsigned i = 0; i < bonded.size(); i++) {
			silicon.push_back(bonded[i]);
		}

		b = bonded.size();
		for (int i = 0; i < b; i++) {
			bonded.erase(bonded.begin());
		}
		break;
	case 3:
		//case for when you want to replace Si-C with Si-Si
		for (unsigned i = 0; i < carbon.size(); i++) {
			if (carbon[i].si_bonded == true) {
				bonded.push_back(carbon[i]);
				carbon.erase(carbon.begin() + i);
			}
		}

		cout << "How many of the " << bonded.size() << " carbons would you like to replace?" << endl;
		cin >> desired_atoms;

		srand(time(NULL));

		for (int i = 0; i < desired_atoms; i++) {
			random = rand() % bonded.size();
			some = bonded[random];
			bonded.erase(bonded.begin() + random);
			silicon.push_back(some);
		}

		for (unsigned i = 0; i < bonded.size(); i++) {
			carbon.push_back(bonded[i]);
		}

		b = bonded.size();
		for (int i = 0; i < b; i++) {
			bonded.erase(bonded.begin());
		}
		break;
	case 4:
		//case for when you want to replace Si-C with C-C
		for (unsigned i = 0; i < silicon.size(); i++) {
			if (silicon[i].c_bonded == true) {
				bonded.push_back(silicon[i]);
				silicon.erase(silicon.begin() + i);
			}
		}

		cout << "How many of the " << bonded.size() << " silicons would you like to replace?" << endl;
		cin >> desired_atoms;

		srand(time(NULL));

		for (int i = 0; i < desired_atoms; i++) {
			random = rand() % bonded.size();
			some = bonded[random];
			bonded.erase(bonded.begin() + random);
			carbon.push_back(some);
		}

		for (unsigned i = 0; i < bonded.size(); i++) {
			silicon.push_back(bonded[i]);
		}

		b = bonded.size();
		for (int i = 0; i < b; i++) {
			bonded.erase(bonded.begin());
		}
		break;
	}

	//printing the POSCAR file
	ofstream fout;
	fout.open("POSCAR");

	fout << system_name << endl;
	fout << lattice_const << endl;
	fout << fixed;
	fout << setprecision(8) << a1[0] << " " << setprecision(8) << a1[1] << " " << setprecision(8) << a1[2] << endl;
	fout << setprecision(8) << a2[0] << " " << setprecision(8) << a2[1] << " " << setprecision(8) << a2[2] << endl;
	fout << setprecision(8) << a3[0] << " " << setprecision(8) << a3[1] << " " << setprecision(8) << a3[2] << endl;
	fout << "C Si H" << endl;
	fout << carbon.size() << " " << silicon.size() << " " << hydrogen.size() << endl;
	fout << coordinate_type << endl;
	for (unsigned i = 0; i < carbon.size(); i++) {
		fout << setprecision(8) << carbon[i].x_direct << " " << setprecision(8) << carbon[i].y_direct << " " << setprecision(8) << carbon[i].z_direct << endl;
	}
	for (unsigned i = 0; i < silicon.size(); i++) {
		fout << setprecision(8) << silicon[i].x_direct << " " << setprecision(8) << silicon[i].y_direct << " " << setprecision(8) << silicon[i].z_direct << endl;
	}
	for (unsigned i = 0; i < hydrogen.size(); i++) {
		fout << setprecision(8) << hydrogen[i].x_direct << " " << setprecision(8) << hydrogen[i].y_direct << " " << setprecision(8) << hydrogen[i].z_direct << endl;
	}

	fout.close();
	fin.close();
	return 0;
}
