#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#ifndef Atom_H
#define Atom_H
using namespace std;


class Atom {
private:
	long double x_cartesian;
	long double y_cartesian;
	long double z_cartesian;
	int bonds = 0;
	vector<Atom> partners;
public:
	Atom();
	bool h_bonded = false;
	bool c_bonded = false;
	bool si_bonded = false;
	long double x_direct;
	long double y_direct;
	long double z_direct;
	long double get_xDirect();
	long double get_yDirect();
	long double get_zDirect();
	long double get_xCartesian();
	long double get_yCartesian();
	long double get_zCartesian();
	void set_cartesian(long double a1[3], long double a2[3], long double a3[3]);
	Atom& operator = (const Atom &a);
};

Atom::Atom() {
	x_cartesian = 0;
	y_cartesian = 0;
	z_cartesian = 0;
	x_direct = 0;
	y_direct = 0;
	z_direct = 0;
	h_bonded = false;
	c_bonded = false;
	si_bonded = false;
	bonds = 0;
}

long double Atom::get_xDirect() {
	return x_direct;
}

long double Atom::get_yDirect() {
	return y_direct;
}

long double Atom::get_zDirect() {
	return z_direct;
}

long double Atom::get_xCartesian() {
	return x_cartesian;
}

long double Atom::get_yCartesian() {
	return y_cartesian;
}

long double Atom::get_zCartesian() {
	return z_cartesian;
}

//for this next function to work, we will need the three lattice vectors from the CONTCAR file
void Atom::set_cartesian(long double a1[3], long double a2[3], long double a3[3]) {
	x_cartesian = (x_direct*(a1[0])) + (y_direct*a2[0]) + (z_direct*a3[0]);
	y_cartesian = (x_direct*(a1[1])) + (y_direct*a2[1]) + (z_direct*a3[1]);
	z_cartesian = (x_direct*(a1[2])) + (y_direct*a2[2]) + (z_direct*a3[2]);
}

Atom& Atom::operator= (const Atom &a) {
	x_cartesian = a.x_cartesian;
	y_cartesian = a.y_cartesian;
	z_cartesian = a.z_cartesian;
	x_direct = a.x_direct;
	y_direct = a.y_direct;
	z_direct = a.z_direct;
	h_bonded = a.h_bonded;
	c_bonded = a.c_bonded;
	si_bonded = a.si_bonded;
	bonds = a.bonds;
	partners = a.partners;

	return *this;
}


long double find_distance(Atom one, Atom two) {
	long double distance;

	long double tempx = one.get_xCartesian() - two.get_xCartesian();
	long double tempx2 = tempx*tempx;

	long double tempy = one.get_yCartesian() - two.get_yCartesian();
	long double tempy2 = tempy*tempy;

	long double tempz = one.get_zCartesian() - two.get_zCartesian();
	long double tempz2 = tempz*tempz;

	long double d = tempx2 + tempy2 + tempz2;

	distance = pow(d, 0.5);

	return distance;
}

#endif