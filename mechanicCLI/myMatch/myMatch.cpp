#include "stdafx.h"
#define _USE_MATH_DEFINES
//#include <math.h>  
#include <typeinfo> 
#using <system.dll> 
#using <mscorlib.dll>
using namespace std;
using namespace System;
using namespace System::IO;

namespace myMatch
{
	static const double um = 0.0000183;
	public enum class axistype { x, y, z, t };
	public enum class IntegrSchems {
		euler, 
		verlet,
		gear
	};
	public enum class Models {
		liner,
		moonriv,
		particle
	};
	public enum class Retypes { stat, dyn };
	public enum class ExtLoadType { 
		none,
		coords,
		displ,
		velos, 
		force 
	};
	public enum class ForcePlace { x, y, xy };
	public enum class AllowSinCos { yes, no };
	public enum class MaterialModels { normal, n0 };
	public enum class PointFreedom { full, locked };
	public enum class CalcTypes { statical, dynamical };
	public value struct point
	{
		ExtLoadType ExtLoad;
		PointFreedom pfreedom;
	};
	public ref class initArr
	{
	public:
		static void Coords(double l, array<point>^ points, array<array<array<double>^>^>^ %arrcoords)
		{
			for (int i = 0; i < arrcoords->Length; i++)
			{
				for (int np = 0; np < points->Length; np++)
				{
					if (np != 0)
					{
						arrcoords[i][np][0] = arrcoords[i][np - 1][0] + l;
					}
					if (points->Length == 2)
					{
						if (np != 0)
						{
							arrcoords[i][np][0] = 0 + l / 2;
						}
						else
						{
							arrcoords[i][np][0] = 0 - l / 2;
						}
					}
				}
			}
		};
		static void _1d(int x, array<double>^ %arr)
		{
			arr = gcnew array<double>(x);
		};
		static void _3d(int y, int z, array<array<array<double>^>^>^ %arr)
		{
			//arr = gcnew array<array<array<double>^>^>(x);
			for (int i = 0; i < arr->Length; i++)
			{
				arr[i] = gcnew array<array<double>^>(y);
				for (int np = 0; np < y; np++)
				{
					arr[i][np] = gcnew array<double>(z);
				}
			}
		};
	};	
	public ref class CosMatrix
	{
	public:
		static double get_lij(array<double>^ x, array<double>^ xp1)
		{
			return sqrt(
				((xp1[0] - x[0]) * (xp1[0] - x[0])) +
				((xp1[1] - x[1]) * (xp1[1] - x[1])) +
				((xp1[2] - x[2]) * (xp1[2] - x[2])));
		}
		/*static double[] get_a1(double lij, array<double>^ x, array<double>^ xp1)
		{
			return new double[3]{
				(xp1[0] - x[0]) / lij,
				(xp1[1] - x[1]) / lij,
				(xp1[2] - x[2]) / lij };
		}*/
		/*static double[] get_a2(double lik, array<double>^] x, double[] k)
		{
			return new double[3]{
				(k[0] - x[0]) / lik,
				(k[1] - x[1]) / lik,
				(k[2] - x[2]) / lik };
		}
		static double[] get_a3(double[] a1, double[] a2)
		{
			return new double[3]{
				a1[1] * a2[2] - a1[2] * a2[1],
				a2[0] * a1[2] - a1[0] * a2[2],
				a2[2] * a1[0] - a2[0] * a1[1] };
		}
		static double[] to_loc(double[] Ux, double[] a1, double[] a2, double[] a3)
		{
			return new double[3]{
				Ux[0] * a1[0] + Ux[1] * a1[1] + Ux[2] * a1[2],
				Ux[0] * a2[0] + Ux[1] * a2[1] + Ux[2] * a2[2],
				Ux[0] * a3[0] + Ux[1] * a3[1] + Ux[2] * a3[2] };
		}*/
	};
	public ref class ForceCalcs
	{
	public:
		static int metal(array<double>^ currUxi, array<double>^ lastUxi, array<double>^ currUxi1, array<double>^ lastUxi1,
			double const Iz, double const A, double const l, double const elastic,
			array<double>^ %F)
		{
			F = gcnew array<double>(3);
			F[0] = ((elastic * A) / l) * (-((currUxi[0] - lastUxi[0]) - (currUxi1[0] - lastUxi1[0])));
			/*F[0] = ((elastic * A) / l) * (-((currUxi[0] - lastUxi[0]) - (currUxi1[0] - lastUxi1[0])));*/

			//F[1] = ((6 * cnst.elastic * Iz / l) - (12 * cnst.elastic * Iz / l * l * l)) * Uy[0] +
			//    6 * cnst.elastic * Iz * Uz[0] / l * l +
			//    12 * cnst.elastic * Iz * Uy[i] / l * l * l -
			//   (6 * cnst.elastic * Iz * Uz[i] / l * l);
			//F[2] = (6 * cnst.elastic * Iz / l * l - (4 * cnst.elastic * Iz)) * Uy[0] -
			//    (4 * cnst.elastic * Iz * Uz[0] / l) -
			//    (6 * cnst.elastic * Iz * Uy[i] / l * l) +
			//    4 * cnst.elastic * Iz * Uz[i] / l;
			return 0;
		};
		static int metal(array<double>^ currxi, array<double>^ lastxi,
			double const Iz, double const A, double const l, double const elastic,
			array<double>^ %F)
		{
			F = gcnew array<double>(3);
			F[0] = ((elastic * A) / l) * (-(currxi[0] - lastxi[0]));
			//F[1] = ((6 * cnst.elastic * Iz / l) - (12 * cnst.elastic * Iz / l * l * l)) * Uy[0] +
			//    6 * cnst.elastic * Iz * Uz[0] / l * l +
			//    12 * cnst.elastic * Iz * Uy[i] / l * l * l -
			//   (6 * cnst.elastic * Iz * Uz[i] / l * l);
			//F[2] = (6 * cnst.elastic * Iz / l * l - (4 * cnst.elastic * Iz)) * Uy[0] -
			//    (4 * cnst.elastic * Iz * Uz[0] / l) -
			//    (6 * cnst.elastic * Iz * Uy[i] / l * l) +
			//    4 * cnst.elastic * Iz * Uz[i] / l;
			return 0;
		};
		static int metalStatic(array<double>^ currxi,
			double const Iz, double const A, double const l, double const elastic,
			array<double>^ %F)
		{
			F = gcnew array<double>(3);
			F[0] = -(((elastic * A) / l) * currxi[0]);
			//F[1] = ((6 * cnst.elastic * Iz / l) - (12 * cnst.elastic * Iz / l * l * l)) * Uy[0] +
			//    6 * cnst.elastic * Iz * Uz[0] / l * l +
			//    12 * cnst.elastic * Iz * Uy[i] / l * l * l -
			//   (6 * cnst.elastic * Iz * Uz[i] / l * l);
			//F[2] = (6 * cnst.elastic * Iz / l * l - (4 * cnst.elastic * Iz)) * Uy[0] -
			//    (4 * cnst.elastic * Iz * Uz[0] / l) -
			//    (6 * cnst.elastic * Iz * Uy[i] / l * l) +
			//    4 * cnst.elastic * Iz * Uz[i] / l;
			return 0;
		};
		static int chorda(array<double>^ currxi, array<double>^ lastxi,
			double A, double l,
			array<double>^ %F)
		{
			F = gcnew array<double>(3);
			array<double>^ lamda1 = gcnew array<double>(3);
			for (int i = 0; i < currxi->Length; i++)
			{
				lamda1[i] = (currxi[i] - lastxi[i]) / l + 1;
				double sigma = 2 * C10 * (lamda1[i] - (1 / (lamda1[i] * lamda1[i]))) + 2 * C01 * (1 - (1 / (lamda1[i] * lamda1[i] * lamda1[i])));
				F[i] = F[i] + A * sigma;
			}
			return 0;
		};
		static int particle(array<double>^ Vm, array<double>^ Vp, double Dp, array<double>^ Re,
			array<double>^ %F)
		{
			F = gcnew array<double>(3);
			for (int i = 0; i < Vm->Length; i++)
			{
				F[i] = 3 * M_PI  * um * Dp * (Vm[i] - Vp[i]) * (1 + (3.0 / 16.0) * Re[i]);
			}
			return 0;
		};

	private:

		static const __int64 elastici = 215000000000;
		static const double C01 = -23512872.8;
		static const double C10 = 22956961.3;
	};
	public ref class Integration
	{
	public:
		static int get_xvab_from_coords(array<double>^ newcoords, array<double>^ lastcoords,
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
			return 0;
		};
		static int get_coordsvab_from_dipl(array<double>^ xp,
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
			return 0;
		};

		static int euler(array<double>^ lastx, array<double>^ lastv, array<double>^ lasta, array<double>^ F,
			double deltat, double massa, double l,
			array<double>^ %xp, array<double>^ %vp, array<double>^ %ap)
		{
			ap = gcnew array<double>(3);
			vp = gcnew array<double>(3);
			xp = gcnew array<double>(3);

			ap[0] = F[0] / massa;
			ap[1] = F[1] / massa;
			ap[2] = F[2] / massa * l * l * 0.5;

			vp[0] = lastv[0] + (ap[0] * deltat);
			vp[1] = lastv[1] + (ap[1] * deltat);
			vp[2] = lastv[2] + (ap[2] * deltat);

			xp[0] = lastx[0] + (vp[0] * deltat);
			xp[1] = lastx[1] + (vp[1] * deltat);
			xp[2] = lastx[2] + (vp[2] * deltat);
			return 0;
		};
		static int verlet(array<double>^  lastx, array<double>^  lastv, array<double>^ lasta, array<double>^  F,
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
			return 0;
		};

		static int gearp(array<double>^ lastx, array<double>^lastv, array<double>^ lasta, array<double>^ lastb, array<double>^ F,
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
			return 0;
		};

		static int gearc(array<double>^ xp, array<double>^ vp, array<double>^ ap, array<double>^ bp, array<double>^ F,
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
			return 0;
		};
	private:
		static const double c0 = 0.1875;
		static const double c1 = 0.697222222;
		static const double c2 = 1;
		static const double c3 = 0.611111111;
	};
	public ref class calc
	{
		static int strtoLoad(String^ str, array<double>^ %Load)
		{
			array<String^>^ arrastr = str->Split(':');
			Load = gcnew array<double>(3) { Convert::ToDouble(arrastr[0]), Convert::ToDouble(arrastr[1]), Convert::ToDouble(arrastr[2]) };
			return 0;
		};
	public:
		static double StrPow(String^ val, String^ pop)
		{
			return Convert::ToDouble(Convert::ToDouble(val) * pow(10, Convert::ToInt32(pop)));
		};
		static void t(int numt, double dt, array<double>^ %t)
		{
			t[0] = 0;
			//t[0] = dt;
			for (int i = 1; i < numt; i++)
			{
				t[i] = t[i - 1] + dt;
			}
		};
		static void Movement(String^ flnameExtLoad, array<point>^ points, Models model, IntegrSchems shema, CalcTypes CalcType, MaterialModels MaterialModeltype, Retypes Retype, bool IsConsoleOut,
			double const L, double const b, double const h, double const ro, int const numP, int const counts, double const elastic, double v0, double const vamp, double const D, double const Renum, array<double>^ time,
			array<array<array<double>^>^>^ %lstF, array<array<array<double>^>^>^ %lstFep1, array<array<array<double>^>^>^ %lstFem1,
			array<array<array<double>^>^>^ %lsta, array<array<array<double>^>^>^ %lstb, array<array<array<double>^>^>^ %lstv, array<array<array<double>^>^>^ %lstdispla, array<array<array<double>^>^>^ %lstcoords,
			array<array<array<double>^>^>^ %lstaAN, array<array<array<double>^>^>^ %lstvAN, array<array<array<double>^>^>^ %lstdisplAN, array<array<array<double>^>^>^ %lstcoordsAN, array<array<array<double>^>^>^ %lstFAN)
		{
			double dt = time[1];
			//double dt = time[0];
			if (IsConsoleOut)
			{
				AllocConsole();
				Console::WriteLine(DateTime::Now);
			}
			//freopen_s("CON", "w", stdout);
			//Console::WriteLine(DateTime::Now);
			double A = b * h;
			double Iz = ((b * h * h * h) / 12);
			if (h == 0 || b == 0)
			{
				A = (M_PI * D * D) / 4;
				Iz = (M_PI * D * D * D * D) / 64;
			}
			double Rp = D / 2;
			double l = L / numP;
			double massa = ro * A * l;
			if (model == Models::particle)
			{
				A = M_PI * pow(Rp, 3) * ((double)4 / (double)3);
			}
			if (IsConsoleOut)
			{
				Console::WriteLine("START");
				Console::WriteLine("A = " + A);
				Console::WriteLine("D = " + D);
				Console::WriteLine("ro = " + ro);
			}
			bool isConsOut = false;
			FileStream^ fs = gcnew FileStream(flnameExtLoad, FileMode::Open);
			StreamReader^ sr = gcnew StreamReader(fs);
			array<array<array<double>^>^>^ Vm = gcnew array<array<array<double>^>^>(time->Length);
			initArr::_3d(numP, 3, Vm);

			for (int linenum = 0; linenum < 4; linenum++)
			{
				sr->ReadLine();
			}
			array<double>^ Load = gcnew array<double>(3);
			//int countCycle = 0;
			double w = 2 * M_PI * 3000;
			double tauP = (2 * ro * Math::Pow(Rp, 2)) / (9 * um);
			double qPs = 1 / Math::Sqrt(1 + (w * tauP) * (w * tauP));
			double ls = (w * tauP) / Math::Sqrt(1 + (w * tauP) * (w * tauP));
			double fiP = Math::Atan(w * tauP);
			for (int k = 0; k < numP; k++)
			{
				double Vm0 = vamp * sin(w * time[0]);
				v0 = qPs * vamp * sin((w * time[0]) - fiP);
				double v_0 = qPs * vamp * sin(- fiP);
				double re0 = Renum;
				if (Retype == Retypes::dyn)
				{
					re0 = (2 * Rp * abs(Vm0 - v0) * ls) / um;
				}
				lstv[0][k][0] = v0;
				lstvAN[0][k][0] = v0;
				//lstF[0][k][0] = 3 * M_PI  * um * D * (Vm0 - v0) * (1 + (3.0 / 16.0) * re0);
				//lstFAN[0][k][0] = lstF[0][k][0];
				//lsta[0][k][0] = qPs * w * vamp * cos((w * time[0]) - fiP);
				//lstF[0][k][0] = lsta[0][k][0] * massa;
				//lsta[0][k][0] = lstF[0][k][0] / massa;
				//lstaAN[0][k][0] = lsta[0][k][0];
				//lstdispla[0][k][0] = - ((qPs * vamp ) / w ) * cos((w * time[0]) - fiP);
				//lstdisplAN[0][k][0] = lstdispla[0][k][0];
			}

			for (int i = 1; i < counts; i++)
			{
				bool predictor = true;
				strtoLoad(sr->ReadLine(), Load);
				for (int np = 0; np < numP; np++)
				{
					l = L / numP;
					massa = ro * A * l;
					if (model == Models::particle)
					{
						massa = ro * A;
					}
					switch (points[np].ExtLoad)
					{
					case ExtLoadType::coords:
						lstcoords[i][np][0] = lstcoords[i][np][0] + Load[0];
						lstcoords[i][np][1] = lstcoords[i][np][1] + Load[1];
						lstcoords[i][np][2] = lstcoords[i][np][2] + Load[2];
						break;
					case ExtLoadType::displ:
						lstdispla[i][np] = Load;
						break;
					case ExtLoadType::velos:
						Vm[i][np] = Load;
						break;
					case ExtLoadType::force:
						lstF[i][np] = Load;
						break;
					}


					array<double>^ Re = gcnew array < double >(3);
					array<double>^ Venv = gcnew array < double >(3);
					double vpredict = lstv[i - 1][np][0] + (lsta[i - 1][np][0] * dt);
					if (model == Models::particle && predictor)
					{
						//countCycle++;

						double qP = 0;
						double lP = 0;
						if (Retype == Retypes::dyn)
						{
							double h = (9 * ro * vamp) / (2 * M_PI * ro * w * Rp);
							qP = (qPs + h * pow(ls, 2)) /( Math::Sqrt(1 + 2 * h * pow(ls, 2) * qPs + pow(h, 2) * pow(ls, 4)));
							lP = ls / Math::Sqrt(1 + 2 * h * pow(ls, 2) * qPs + pow(h, 2) * pow(ls, 4));
						}
						else if (Retype == Retypes::stat)
						{
							qP = qPs;
							lP = ls;
						}
						//qP = qPs;
						if (!isConsOut)
						{
							Console::WriteLine("massa = " + massa);
							Console::WriteLine("tauP = " + tauP);
							Console::WriteLine("qP = " + qP);
							Console::WriteLine("fiP = " + fiP);
							Console::WriteLine("END");
							isConsOut = true;
						}
						Venv = gcnew array < double >(3);						
						if (numP > 1)
						{
							double ni = um / 1.2754;
							double r = abs(lstcoords[i][0][0] - lstcoords[i][1][0]);
							double cosTeta = 0;
							if (np == 0)
							{
								cosTeta = -1;
							}
							if (np == 1)
							{
								cosTeta = 1;
							}
							double Vk = Vm[i][np][0] - lstv[i][np][0];
							double moduleVk = abs(Vk);
							double Ak = ((3 * ni * Rp * Vk) / (2 * moduleVk)) * ( 1 + ((3 * ni * Rp * moduleVk) / (8 * ni)));
							double Vmp = (Ak / pow(r, 2)) - (Ak / pow(r, 2)) * exp(-(r / (2 * ni)) * ( moduleVk - Vk * cosTeta)) * (1 + (r / (2 * ni)) * (moduleVk + Vk * cosTeta));
							//Vmp = Vmp / 10;
							Venv[0] = Vm[i][np][0] + Vmp;
							lstvAN[i][np][0] = lstvAN[i][np][0] + qP * ((vamp * sin((w *  time[i]) - fiP)) + Vmp);
							lstaAN[i][np][0] = lstaAN[i][np][0] + qP * ((vamp * cos((w *  time[i]) - fiP) * w) + Vmp);
						}
						else
						{
							Venv[0] = Vm[i][np][0];
							lstdisplAN[i][np][0] = lstdisplAN[i][np][0] - ((qPs * vamp) / w) * cos((w *  time[i]) - fiP);
							lstvAN[i][np][0] = lstvAN[i][np][0] + qPs * vamp * sin((w *  time[i]) - fiP);
							lstaAN[i][np][0] = lstaAN[i][np][0] + qPs * vamp * cos((w *  time[i]) - fiP) * w;
							lstFAN[i][np][0] = lstaAN[i][np][0] * massa;
						}
						if (Retype == Retypes::dyn)
						{
							for (int j = 0; j < 3; j++)
							{
								Re[j] = (2 * Rp * abs(Venv[0] - lstv[i][np][0]) * lP) / um;
							}
						}
						else
						{
							for (int j = 0; j < 3; j++)
							{
								Re[j] = Renum;
							}
						}
					}

					switch (model)
					{
					case Models::liner:
						ForceCalcs::metal(lstcoords[i][np], lstcoords[i - 1][np], Iz, A, l, elastic, lstFep1[i][np]);
						switch (MaterialModeltype)
						{
						case MaterialModels::n0:
							if (lstFem1[i][np][0] < 0)
							{
								lstFem1[i][np][0] = 0;
							}
							break;
						}
						lstF[i][np][0] = lstF[i][np][0] + lstFem1[i][np][0] + lstFep1[i][np][0];
						lstF[i][np][1] = lstF[i][np][1] + lstFem1[i][np][1] + lstFep1[i][np][1];
						lstF[i][np][2] = lstF[i][np][2] + lstFem1[i][np][2] + lstFep1[i][np][2];
						break;
					case Models::moonriv:
						ForceCalcs::chorda(lstcoords[i][np], lstcoords[i - 1][np], A, l, lstFep1[i][np]);
						switch (MaterialModeltype)
						{
						case MaterialModels::n0:
							if (lstFem1[i][np][0] < 0)
							{
								lstFem1[i][np][0] = 0;
							}
							break;
						}
						lstF[i][np][0] = lstF[i][np][0] + lstFem1[i][np][0] + lstFep1[i][np][0];
						lstF[i][np][1] = lstF[i][np][1] + lstFem1[i][np][1] + lstFep1[i][np][1];
						lstF[i][np][2] = lstF[i][np][2] + lstFem1[i][np][2] + lstFep1[i][np][2];
						break;
					case Models::particle:
						lstv[i][np][0] = lstv[i - 1][np][0] + (lsta[i - 1][np][0] * dt);
						ForceCalcs::particle(Venv, lstv[i][np], D, Re, lstF[i][np]);
						break;
					}	

					switch (points[np].ExtLoad)
					{
					case ExtLoadType::coords:
						Integration::get_xvab_from_coords(lstcoords[i][np], lstcoords[i - 1][np], lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], dt,
							lstdispla[i][np], lstv[i][np], lsta[i][np]);
						break;
					case ExtLoadType::displ:
						Integration::get_coordsvab_from_dipl(lstdispla[i][np], lstcoords[i - 1][np], lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], dt,
							lstdispla[i][np], lstv[i][np], lsta[i][np]);
						break;
					default:
						switch (shema)
						{
						case IntegrSchems::euler:
							Integration::euler(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstF[i][np], dt, massa, l,
								lstdispla[i][np], lstv[i][np], lsta[i][np]);
							break;
						case IntegrSchems::verlet:
							Integration::verlet(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstF[i][np], dt, massa, l,
								lstdispla[i][np], lstv[i][np], lsta[i][np]);
							break;
						case IntegrSchems::gear:
							if (predictor)
							{
								Integration::gearp(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstb[i - 1][np], lstF[i - 1][np], dt, massa, l,
									lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np]);
								break;
							}
							else
							{
								Integration::gearc(lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np], lstF[i][np], dt, massa, l,
									lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np]);
								break;
							}
						}
						lstcoords[i][np][0] = lstcoords[0][np][0] + lstdispla[i][np][0];
						lstcoords[i][np][1] = lstcoords[0][np][1] + lstdispla[i][np][1];
						lstcoords[i][np][2] = lstcoords[0][np][2] + lstdispla[i][np][2];
						break;
					}
					if (np == numP - 1)
					{
						//lstFem1[i][np][0] = 0 - ForceCalcs.metal(lstdispla[i][np - 1], lstdispla[i - 1][np - 1], Iz, A, l)[0];
						if (shema == IntegrSchems::gear && predictor)
						{
							predictor = false;
							np = -1;
						}
					}
				}
			}
			sr->Close();
			fs->Close();
		};
		static void StaticMovement(String^ flnameExtLoad, array<point>^ points, Models model, IntegrSchems shema, CalcTypes CalcType, MaterialModels MaterialModeltype, Retypes Retype, bool IsConsoleOut,
			double const L, double const b, double const h, double const ro, int const numP, int const counts, double const elastic, double const v0, double const vamp, double const D, double const Renum, array<double>^ time,
			array<array<array<double>^>^>^ %lstF, array<array<array<double>^>^>^ %lstFep1, array<array<array<double>^>^>^ %lstFem1,
			array<array<array<double>^>^>^ %lsta, array<array<array<double>^>^>^ %lstb, array<array<array<double>^>^>^ %lstv, array<array<array<double>^>^>^ %lstdispla, array<array<array<double>^>^>^ %lstcoords, array<array<array<double>^>^>^ %lstvAN)
		{
			double dt = time[1];
			if (IsConsoleOut)
			{
				AllocConsole();
				Console::WriteLine(DateTime::Now);
			}
			//freopen_s("CON", "w", stdout);
			//Console::WriteLine(DateTime::Now);
			double A = b * h;
			double Iz = ((b * h * h * h) / 12);
			if (h == 0 || b == 0)
			{
				A = (M_PI * D * D) / 4;
				Iz = (M_PI * D * D * D * D) / 64;
			}
			double r = D / 2;
			if (model == Models::particle)
			{
				A = M_PI * (r * r * r) * ((double)4 / (double)3);
			}
			if (IsConsoleOut)
			{
				Console::WriteLine("START");
				Console::WriteLine("A = " + A);
				Console::WriteLine("D = " + D);
				Console::WriteLine("ro = " + ro);
			}
			bool isConsOut = false;
			FileStream^ fs = gcnew FileStream(flnameExtLoad, FileMode::Open);
			StreamReader^ sr = gcnew StreamReader(fs);
			array<array<array<double>^>^>^ Vm = gcnew array<array<array<double>^>^>(time->Length);
			initArr::_3d(numP, 3, Vm);

			for (int linenum = 0; linenum < 4; linenum++)
			{
				sr->ReadLine();
			}
			lstv[0][0][0] = v0;
			lstvAN[0][0][0] = v0;
			array<double>^ Load = gcnew array<double>(3);
			//int countCycle = 0;

			for (int i = 1; i < counts; i++)
			{
				bool predictor = true;
				strtoLoad(sr->ReadLine(), Load);
				for (int np = 0; np < numP; np++)
				{
					double l = L / numP;
					double massa = ro * A * l;
					if (model == Models::particle)
					{
						massa = ro * A;
					}

					if (points[np].ExtLoad != ExtLoadType::none)
					{
						switch (points[np].ExtLoad)
						{
						case ExtLoadType::coords:
							lstcoords[i][np][0] = lstcoords[i][np][0] + Load[0];
							lstcoords[i][np][1] = lstcoords[i][np][1] + Load[1];
							lstcoords[i][np][2] = lstcoords[i][np][2] + Load[2];
							break;
						case ExtLoadType::displ:
							lstdispla[i][np] = Load;
							break;
						case ExtLoadType::velos:
							Vm[i][np] = Load;
							break;
						case ExtLoadType::force:
							lstF[i][np] = Load;
							break;
						}
					}
					switch (points[np].ExtLoad)
					{
					case ExtLoadType::coords:
						Integration::get_xvab_from_coords(lstcoords[i][np], lstcoords[i - 1][np], lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], dt,
							lstdispla[i][np], lstv[i][np], lsta[i][np]);
						break;
					case ExtLoadType::displ:
						Integration::get_coordsvab_from_dipl(lstdispla[i][np], lstcoords[i - 1][np], lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], dt,
							lstdispla[i][np], lstv[i][np], lsta[i][np]);
						break;
					default:
						switch (shema)
						{
						case IntegrSchems::euler:
							Integration::euler(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstF[i - 1][np], dt, massa, l,
								lstdispla[i][np], lstv[i][np], lsta[i][np]);
							break;
						case IntegrSchems::verlet:
							Integration::verlet(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstF[i - 1][np], dt, massa, l,
								lstdispla[i][np], lstv[i][np], lsta[i][np]);
							break;
						case IntegrSchems::gear:
							if (predictor)
							{
								Integration::gearp(lstdispla[i - 1][np], lstv[i - 1][np], lsta[i - 1][np], lstb[i - 1][np], lstF[i - 1][np], dt, massa, l,
									lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np]);
								break;
							}
							else
							{
								Integration::gearc(lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np], lstF[i][np], dt, massa, l,
									lstdispla[i][np], lstv[i][np], lsta[i][np], lstb[i][np]);
								break;
							}
						}
						lstcoords[i][np][0] = lstcoords[i - 1][np][0] + lstdispla[i][np][0];
						lstcoords[i][np][1] = lstcoords[i - 1][np][1] + lstdispla[i][np][1];
						lstcoords[i][np][2] = lstcoords[i - 1][np][2] + lstdispla[i][np][2];
						break;
					}

					array<double>^ Re = gcnew array < double >(3);
					array<double>^ Venv = gcnew array < double >(3);
					array<double>^ currUx = lstdispla[i][np];
					array<double>^ lastUx = lstdispla[i - 1][np];

					if (model == Models::particle && predictor)
					{
						if (Retype == Retypes::dyn)
						{
							for (int j = 0; j < 3; j++)
							{
								Re[j] = (lstv[i][np][j] * D) / um;
							}
						}
						else if (Retype == Retypes::stat)
						{
							for (int j = 0; j < 3; j++)
							{
								Re[j] = Renum;
							}
						}
						//countCycle++;
						double w = 2 * M_PI * 3000;
						Venv = gcnew array < double >(3);
						Venv[0] = Vm[i][np][0];
						double tauP = (ro * Math::Pow(D, 2)) / (18 * um);
						double qPs = 1 / Math::Sqrt(1 + (w * tauP) * (w * tauP));
						double ls = (w * tauP) / Math::Sqrt(1 + (w * tauP) * (w * tauP));
						double qP = 0;
						if (Retype == Retypes::dyn)
						{
							double h = (9 * ro * vamp) / (2 * M_PI * ro * w * D);
							qP = (qPs + h * (ls * ls)) / Math::Sqrt(1 + 2 * h * (ls * ls) * qPs + (h * h) * (ls * ls* ls* ls));
						}
						else if (Retype == Retypes::stat)
						{
							qP = qPs;
						}
						double fiP = Math::Atan(w * tauP);
						if (!isConsOut)
						{
							Console::WriteLine("massa = " + massa);
							Console::WriteLine("tauP = " + tauP);
							Console::WriteLine("qP = " + qP);
							Console::WriteLine("fiP = " + fiP);
							Console::WriteLine("END");
							isConsOut = true;
						}
						lstvAN[i][np][0] = lstvAN[i][np][0] + qP * vamp * Math::Sin((w *  time[i]) - fiP);
					}

					if (np == 0)
					{
						//l = Math.Sqrt(pow2(lstcoords[i][np][0] - lstcoords[i][np + 1][0]) - pow2(lstcoords[i][np][1] - lstcoords[i][np + 1][1]));
						if (CalcType == CalcTypes::statical)
						{
							switch (model)
							{
							case Models::liner:
								ForceCalcs::metal(currUx, lastUx, Iz, A, l, elastic, lstFep1[i][np]);
								break;
							case Models::moonriv:
								ForceCalcs::chorda(currUx, lastUx, A, l, lstFep1[i][np]);
								break;
							case Models::particle:
								ForceCalcs::particle(Venv, lstv[i][np], D, Re, lstFep1[i][np]);
								break;
							default:
								break;
							}
						}
						else
						{
							switch (model)
							{
							case Models::liner:
								ForceCalcs::metal(lstcoords[i][np], lstcoords[i - 1][np], Iz, A, l, elastic, lstFep1[i][np]);
								break;
							case Models::moonriv:
								ForceCalcs::chorda(lstcoords[i][np], lstcoords[i - 1][np], A, l, lstFep1[i][np]);
								break;
							case Models::particle:
								ForceCalcs::particle(Venv, lstv[i][np], D, Re, lstFep1[i][np]);
								break;
							default:
								break;
							}
						}
					}
					else if (np == numP - 1)
					{
						//lstFem1[i][np][0] = 0 - ForceCalcs.metal(lstdispla[i][np - 1], lstdispla[i - 1][np - 1], Iz, A, l)[0];
					}
					else
					{
						if (CalcType == CalcTypes::statical)
						{

							switch (model)
							{
							case Models::liner:
								ForceCalcs::metal(currUx, lastUx, Iz, A, l, elastic, lstFep1[i][np]);
								break;
							case Models::moonriv:
								ForceCalcs::chorda(currUx, lastUx, A, l, lstFep1[i][np]);
								break;
							case Models::particle:
								break;
							default:
								break;
							}
						}
						else
						{
							switch (model)
							{
							case Models::liner:
								ForceCalcs::metal(lstcoords[i][np], lstcoords[i - 1][np], Iz, A, l, elastic, lstFep1[i][np]);
								break;
							case Models::moonriv:
								ForceCalcs::chorda(lstcoords[i][np], lstcoords[i - 1][np], A, l, lstFep1[i][np]);
								break;
							case Models::particle:
								break;
							default:
								break;
							}
						}
						lstFem1[i][np][0] = 0 - lstFep1[i][np - 1][0];
						lstFem1[i][np][1] = 0 - lstFep1[i][np - 1][1];
					}
					switch (MaterialModeltype)
					{
					case MaterialModels::n0:
						if (lstFem1[i][np][0] < 0)
						{
							lstFem1[i][np][0] = 0;
						}
						break;
					}
					lstF[i][np][0] = lstF[i][np][0] + lstFem1[i][np][0] + lstFep1[i][np][0];
					lstF[i][np][1] = lstF[i][np][1] + lstFem1[i][np][1] + lstFep1[i][np][1];
					lstF[i][np][2] = lstF[i][np][2] + lstFem1[i][np][2] + lstFep1[i][np][2];
					if (np == numP - 1)
					{
						//lstFem1[i][np][0] = 0 - ForceCalcs.metal(lstdispla[i][np - 1], lstdispla[i - 1][np - 1], Iz, A, l)[0];
						if (shema == IntegrSchems::gear && predictor)
						{
							predictor = false;
							np = -1;
						}
					}
				}
			}
			sr->Close();
			fs->Close();
		};
	};
	public ref class ExtLoad
	{
	public:
		static ExtLoadType getPoint(String^ ExtIn)
		{
			if (ExtIn == "coords")
			{
				return ExtLoadType::coords;
			}
			else if (ExtIn == "displ")
			{
				return ExtLoadType::displ;
			}
			else if (ExtIn == "force")
			{
				return ExtLoadType::force;
			}
			else if (ExtIn == "velos")
			{
				return ExtLoadType::velos;
			}
			else
			{
				return ExtLoadType::none;
			}
		}
		/*
		//int readlarge()
		//{
		//	String^ path = L"F:\\MyTestFile0.txt";

		//	try
		//	{
		//		if (File::Exists(path))
		//		{
		//			File::Delete(path);
		//		}

		//		//Console::WriteLine(L"Writing some text to {0}", path);
		//		StreamWriter^ sw = gcnew StreamWriter(path);
		//		sw->WriteLine("Thiqwsqs");
		//		sw->WriteLine("is some text");
		//		sw->WriteLine("written to test");
		//		sw->WriteLine("for reading...");
		//		sw->Close();

		//		//Console::WriteLine("Reading the previously written text..");
		//		FileStream^ fs = gcnew FileStream(path, FileMode::Open);
		//		StreamReader^ sr = gcnew StreamReader(fs);

		//		while (sr->Peek() >= 0)
		//		{
		//			Console::WriteLine(sr->ReadLine());
		//		}
		//	}
		//	catch (Exception^ e)
		//	{
		//		Console::WriteLine("The process failed: {0}", e->ToString());
		//		return 1;
		//	}
		//	return 0;
		//};
		//static int getStr(String^ fileName, array<String^>^ %strLoad)
		//{
		//	try
		//	{
		//		String^ delimStr = "\n";
		//		array<Char>^ delimiter = delimStr->ToCharArray();
		//		StreamReader^ din = File::OpenText(fileName);
		//		String^ outstr = din->ReadToEnd();
		//		strLoad = outstr->Split(delimiter);
		//	}
		//	catch (Exception^ e)
		//	{
		//		if (dynamic_cast<FileNotFoundException^>(e))
		//			return 404;
		//		//Console::WriteLine("file '{0}' not found", fileName);
		//		else
		//			return 1;
		//		//Console::WriteLine("problem reading file '{0}'", fileName);
		//	}
		//	return 0;
		//};
		//static void getNum(array<String^>^ strLoad, array<int>^ %findIndex)
		//{
		//	String^ numLoad = "";
		//	for (int i = 0; i < strLoad->Length; i++)
		//	{
		//		if (strLoad[i]->Contains("loadstart:"))
		//		{
		//			if (numLoad != "")
		//			{
		//				numLoad = numLoad + ":" + i.ToString();
		//			}
		//			else
		//			{
		//				numLoad = i.ToString();
		//			}
		//		}
		//	}
		//	array<String^>^ arrnumLoad = numLoad->Split(':');
		//	findIndex = gcnew array<int>(arrnumLoad->Length);
		//	for (int i = 0; i < arrnumLoad->Length; i++)
		//	{
		//		findIndex[i] = Convert::ToInt32(arrnumLoad[i]);
		//	}
		//};
		//static void getExtLoad(array<String^>^ strLoad, array<int>^ findIndex, int counts, array<point>^ %points, array<array<array<double>^>^>^ %arrLoad)
		//{
		//	for (int ifi = 0; ifi < findIndex->Length; ifi++)
		//	{
		//		array<String^>^ currLoad = strLoad[findIndex[ifi]]->Split(':');
		//		int numberp = Convert::ToInt32(currLoad[1]);
		//		points[numberp].ExtLoad = getPoint(currLoad[2]);
		//		arrLoad[numberp] = gcnew array<array<double>^>(counts);
		//		int it = 1;
		//		while (strLoad[findIndex[ifi] + it] != "loadend")
		//		{
		//			array<String^>^ arraystr = strLoad[findIndex[ifi] + it]->Split(':');
		//			arrLoad[numberp][it - 1] = gcnew array<double> { Convert::ToDouble(arraystr[0]), Convert::ToDouble(arraystr[1]), Convert::ToDouble(arraystr[2]) };
		//			it++;
		//		}
		//	}
		//};
		//static int putExtLoad(double l, array<array<array<double>^>^>^ arrLoad, array<point>^ points, array<array<array<double>^>^>^ %arrF, array<array<array<double>^>^>^ %arrdispl, array<array<array<double>^>^>^ %arrcoords)
		//{
		//	for (int i = 0; i < arrF->Length; i++)
		//	{
		//		for (int np = 0; np < points->Length; np++)
		//		{
		//			if (np != 0)
		//			{
		//				arrcoords[i][np][0] = arrcoords[i][np - 1][0] + l;
		//			}
		//			switch (points[np].ExtLoad)
		//			{
		//			case ExtLoadType::coords:
		//				for (int j = 0; j < arrcoords[i][np]->Length; j++)
		//				{
		//					arrcoords[i][np][j] = arrcoords[i][np][j] + arrLoad[np][i][j];
		//				}
		//				break;
		//			case ExtLoadType::displ:
		//				arrdispl[i][np] = arrLoad[np][i];
		//				break;
		//			case ExtLoadType::force:
		//				arrF[i][np] = arrLoad[np][i];
		//				break;
		//			}
		//		}
		//	}
		//	return 0;
		//};
		//int saveStrFile(String^ fileName, String^ str)
		//{
		//	try
		//	{
		//		File::WriteAllText(fileName, str);
		//	}
		//	catch (Exception^ e)
		//	{
		//		if (dynamic_cast<FileNotFoundException^>(e))
		//			return 404;
		//		//Console::WriteLine("file '{0}' not found", fileName);
		//		else
		//			return 1;
		//		//Console::WriteLine("problem reading file '{0}'", fileName);
		//	}
		//	return 0;
		//};
		//static int saveStrFile(String^ fileName, array<String^>^ str)
		//{
		//	try
		//	{
		//		File::WriteAllLines(fileName, str);
		//	}
		//	catch (Exception^ e)
		//	{
		//		if (dynamic_cast<FileNotFoundException^>(e))
		//			return 404;
		//		//Console::WriteLine("file '{0}' not found", fileName);
		//		else
		//			return 1;
		//		//Console::WriteLine("problem reading file '{0}'", fileName);
		//	}
		//	return 0;
		//};
		*/
	};
}