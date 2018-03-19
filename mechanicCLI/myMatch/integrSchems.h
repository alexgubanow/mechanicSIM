#pragma once
public ref class integrSchems
{
public:
	static void get_xvab_from_coords(array<double>^ newcoords, array<double>^ lastcoords,
		array<double>^ lastx, array<double>^ lastv, array<double>^ lasta,
		double dt, array<double>^ %xp, array<double>^ %vp, array<double>^ %ap);
	static void get_coordsvab_from_dipl(array<double>^ xp,
		array<double>^ lastcoords, array<double>^ lastx, array<double>^ lastv, array<double>^ lasta,
		double dt, array<double>^ %coords, array<double>^  %vp, array<double>^ %ap);
	static void euler(array<double>^ lastx, array<double>^ lastv, array<double>^ lasta, array<double>^ F,
		double deltat, double massa, double l,
		array<double>^ %xp, array<double>^ %vp, array<double>^ %ap);
	static void verlet(array<double>^  lastx, array<double>^  lastv, array<double>^ lasta, array<double>^  F,
		double deltat, double massa, double l,
		array<double>^ %xp, array<double>^  %vp, array<double>^  %ap);
	static void gearp(array<double>^ lastx, array<double>^lastv, array<double>^ lasta, array<double>^ lastb, array<double>^ F,
		double deltat, double massa, double l,
		array<double>^ %xp, array<double>^ %vp, array<double>^ %ap, array<double>^ %bp);
	static void gearc(array<double>^ xp, array<double>^ vp, array<double>^ ap, array<double>^ bp, array<double>^ F,
		double deltat, double massa, double l,
		array<double>^ %xc, array<double>^ %vc, array<double>^ %ac, array<double>^ %bc);
};