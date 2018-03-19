#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <typeinfo>
#using <system.dll>
#using <mscorlib.dll>
#include "integrSchems.h"

using namespace std;
using namespace System;
using namespace System::IO;

static const double c0 = 0.1875;
static const double c1 = 0.697222222;
static const double c2 = 1;
static const double c3 = 0.611111111;

void integrSchems::get_xvab_from_coords(array<double>^ newcoords, array<double>^ lastcoords,
	array<double>^ lastx, array<double>^ lastv, array<double>^ lasta,
	double dt, array<double>^ %xp, array<double>^ %vp, array<double>^ %ap)
{
	ap = gcnew array<double>(3);
	vp = gcnew array<double>(3);
	xp = gcnew array<double>(3);

	xp[0] = lastx[0] + (newcoords[0] - lastcoords[0]) / dt;
	xp[1] = lastx[1] + (newcoords[0] - lastcoords[0]) / dt;
	xp[2] = lastx[2] + (newcoords[0] - lastcoords[0]) / dt;

	vp[0] = lastv[0] + (xp[0] - lastx[0]) / dt;
	vp[1] = lastv[1] + (xp[1] - lastx[1]) / dt;
	vp[2] = lastv[2] + (xp[2] - lastx[2]) / dt;

	ap[0] = lasta[0] + (vp[0] - lastv[0]) / dt;
	ap[1] = lasta[1] + (vp[1] - lastv[1]) / dt;
	ap[2] = lasta[2] + (vp[2] - lastv[2]) / dt;
};
void integrSchems::get_coordsvab_from_dipl(array<double>^ xp,
	array<double>^ lastcoords, array<double>^ lastx, array<double>^ lastv, array<double>^ lasta,
	double dt, array<double>^ %coords, array<double>^  %vp, array<double>^ %ap)
{
	ap = gcnew array<double>(3);
	vp = gcnew array<double>(3);
	coords = gcnew array<double>(3);

	coords[0] = xp[0] + lastcoords[0];
	coords[1] = xp[1] + lastcoords[1];
	coords[2] = xp[2] + lastcoords[2];

	vp[0] = lastv[0] + (xp[0] - lastx[0]) / dt;
	vp[1] = lastv[1] + (xp[1] - lastx[1]) / dt;
	vp[2] = lastv[2] + (xp[2] - lastx[2]) / dt;

	ap[0] = lasta[0] + (vp[0] - lastv[0]) / dt;
	ap[1] = lasta[1] + (vp[1] - lastv[1]) / dt;
	ap[2] = lasta[2] + (vp[2] - lastv[2]) / dt;
};

void integrSchems::euler(array<double>^ lastx, array<double>^ lastv, array<double>^ lasta, array<double>^ F,
	double deltat, double massa, double l,
	array<double>^ %xp, array<double>^ %vp, array<double>^ %ap)
{
	ap = gcnew array<double>(3);
	vp = gcnew array<double>(3);
	xp = gcnew array<double>(3);

	ap[0] = F[0] / massa;
	ap[1] = F[1] / massa;
	ap[2] = F[2] / massa * l * l * 0.5;

	vp[0] = lastv[0] + (lasta[0] * deltat);
	vp[1] = lastv[1] + (lasta[1] * deltat);
	vp[2] = lastv[2] + (lasta[2] * deltat);

	xp[0] = lastx[0] + (lastv[0] * deltat);
	xp[1] = lastx[1] + (lastv[1] * deltat);
	xp[2] = lastx[2] + (lastv[2] * deltat);
};
void integrSchems::verlet(array<double>^  lastx, array<double>^  lastv, array<double>^ lasta, array<double>^  F,
	double deltat, double massa, double l,
	array<double>^ %xp, array<double>^  %vp, array<double>^  %ap)
{
	ap = gcnew array<double>(3);
	vp = gcnew array<double>(3);
	xp = gcnew array<double>(3);

	ap[0] = F[0] / massa;
	ap[1] = F[1] / massa;
	ap[2] = F[2] / massa * l * l * 0.5;

	vp[0] = lastv[0] + 0.5 * (lasta[0] + ap[0]) * deltat;
	vp[1] = lastv[1] + 0.5 * (lasta[1] + ap[1]) * deltat;
	vp[2] = lastv[2] + 0.5 * (lasta[2] + ap[2]) * deltat;

	xp[0] = lastx[0] + (lastv[0] * deltat) + 0.5 * lasta[0] * deltat * deltat;
	xp[1] = lastx[1] + (lastv[1] * deltat) + 0.5 * lasta[1] * deltat * deltat;
	xp[2] = lastx[2] + (lastv[2] * deltat) + 0.5 * lasta[2] * deltat * deltat;
};

void integrSchems::gearp(array<double>^ lastx, array<double>^lastv, array<double>^ lasta, array<double>^ lastb, array<double>^ F,
	double deltat, double massa, double l,
	array<double>^ %xp, array<double>^ %vp, array<double>^ %ap, array<double>^ %bp)
{
	bp = gcnew array<double>(3);
	ap = gcnew array<double>(3);
	vp = gcnew array<double>(3);
	xp = gcnew array<double>(3);
	//array<double>^ bp = gcnew array<double>(3);

	bp[0] = ((F[0] / massa) - lasta[0]) / deltat;
	bp[1] = ((F[1] / massa) - lasta[1]) / deltat;
	bp[2] = ((F[2] / massa * l * l * 0.5) - lasta[2]) / deltat;

	ap[0] = lasta[0] + lastb[0] * deltat;
	ap[1] = lasta[1] + lastb[1] * deltat;
	ap[2] = lasta[2] + lastb[2] * deltat;

	vp[0] = lastv[0] + lasta[0] * deltat + 1 / 2 * lastb[0] * deltat * deltat;
	vp[1] = lastv[1] + lasta[1] * deltat + 1 / 2 * lastb[1] * deltat * deltat;
	vp[2] = lastv[2] + lasta[2] * deltat + 1 / 2 * lastb[2] * deltat * deltat;

	xp[0] = lastx[0] + lastv[0] * deltat + 1 / 2 * lasta[0] * deltat * deltat + 1 / 6 * lastb[0] * deltat * deltat * deltat;
	xp[1] = lastx[1] + lastv[1] * deltat + 1 / 2 * lasta[1] * deltat * deltat + 1 / 6 * lastb[1] * deltat * deltat * deltat;
	xp[2] = lastx[2] + lastv[2] * deltat + 1 / 2 * lasta[2] * deltat * deltat + 1 / 6 * lastb[2] * deltat * deltat * deltat;
};

void integrSchems::gearc(array<double>^ xp, array<double>^ vp, array<double>^ ap, array<double>^ bp, array<double>^ F,
	double deltat, double massa, double l,
	array<double>^ %xc, array<double>^ %vc, array<double>^ %ac, array<double>^ %bc)
{
	bc = gcnew array<double>(3);
	ac = gcnew array<double>(3);
	vc = gcnew array<double>(3);
	xc = gcnew array<double>(3);

	ac[0] = F[0] / massa;
	ac[1] = F[1] / massa;
	ac[2] = F[2] / massa * l * l * 0.5;

	array<double>^ deltaa = gcnew array<double>(3);

	deltaa[0] = ac[0] - ap[0];
	deltaa[1] = ac[1] - ap[1];
	deltaa[2] = ac[2] - ap[2];

	xc[0] = xp[0] + c0 * deltaa[0];
	xc[1] = xp[1] + c0 * deltaa[1];
	xc[2] = xp[2] + c0 * deltaa[2];

	vc[0] = vp[0] + c1 * deltaa[0];
	vc[1] = vp[1] + c1 * deltaa[1];
	vc[2] = vp[2] + c1 * deltaa[2];

	ac[0] = ap[0] + c2 * deltaa[0];
	ac[1] = ap[1] + c2 * deltaa[1];
	ac[2] = ap[2] + c2 * deltaa[2];

	bc[0] = bp[0] + c3 * deltaa[0];
	bc[1] = bp[1] + c3 * deltaa[1];
	bc[2] = bp[2] + c3 * deltaa[2];
};