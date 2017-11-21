#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>  
#using <system.dll> 
#using <mscorlib.dll>
using namespace std;
using namespace System;
using namespace System::IO;

namespace myMath
{
	public enum class forceTypes
	{
		zeros,
		k_x,
		sin_x,
		mod_sin,
		rev_mod_sin,
		cos_x
	};
	public enum class ExtLoadTypes {
		none,
		coords,
		displ,
		velos,
		force
	};
	public enum class forceCycleTypes
	{
		Full,
		Half,
		_3_4
	};

	public value struct forcePlaces
	{
		bool x;
		bool y;
		bool z;
	};

	public value struct forcePoints
	{
		int nump;
		double freq;
		double Amp;
		double k;
		forcePlaces myforcePlaces;
		forceCycleTypes forceCycle;
		forceTypes forceType;
		ExtLoadTypes ExtLoadType;
	};
	public ref class initArr
	{
	public:
		static void _1d(int x, array<double>^ %arr)
		{
			arr = gcnew array<double>(x);
		};
		static void _3d(int x, int y, int z, array<array<array<double>^>^>^ %arr)
		{
			//arr = gcnew array<array<array<double>^>^>(x);
			for (int i = 0; i < x; i++)
			{
				arr[i] = gcnew array<array<double>^>(y);
				for (int np = 0; np < y; np++)
				{
					arr[i][np] = gcnew array<double>(z);
				}
			}
		};
	};

	public ref class calc
	{
		static void t(int numt, double dt, array<double>^ %t)
		{
			//t = gcnew array<double>(numt);
			t[0] = 0;
			for (int i = 1; i < numt; i++)
			{
				t[i] = t[i - 1] + dt;
			}
		};
		static double F(forceTypes forceType, double w, double Amp, double k, double t)
		{
			switch (forceType)
			{
			case forceTypes::k_x:
				return Amp * k * t;
			case forceTypes::sin_x:
				return Amp * sin(w * t);
			case forceTypes::cos_x:
				return Amp * cos(w * t);
			case forceTypes::mod_sin:
				return Amp * fabs(sin(w * t));
			case forceTypes::rev_mod_sin:
				return Amp * (-fabs(sin(w * t)));
			case forceTypes::zeros:
				return 0;
			default:
				return 0;
			}
		};
		static int maxT(forceCycleTypes forceCycle, int numt)
		{
			switch (forceCycle)
			{
			case forceCycleTypes::Full:
				return numt;
			case forceCycleTypes::Half:
				return numt / 2;
			case forceCycleTypes::_3_4:
				return numt - (numt / 4);
			default:
				return numt;
			}
		}
	public:
		static double StrPow(String^ val, String^ pop)
		{
			return Convert::ToDouble(Convert::ToDouble(val) * pow(10, Convert::ToInt32(pop)));
		};
		static int StrPowInt(String^ val, String^ pop)
		{
			return Convert::ToInt32(Convert::ToInt32(val) * pow(10, Convert::ToInt32(pop)));
		};
		static void ExtLoad(array<forcePoints>^ myFPs, int numt, double dt, array<array<array<double>^>^>^ %lstFex)
		{
			array<double>^ tArr = gcnew array<double>(numt);
			t(numt, dt, tArr);
			for (int p = 0; p < myFPs->Length; p++)
			{
				int endT = maxT(myFPs[p].forceCycle, numt);
				for (int i = 0; i < endT; i++)
				{

					double w = 2 * M_PI * myFPs[p].freq;
					if (myFPs[p].myforcePlaces.x == true)
					{
						lstFex[i][p][0] = F(myFPs[p].forceType, w, myFPs[p].Amp, myFPs[p].k, tArr[i]);
					}
					if (myFPs[p].myforcePlaces.y == true)
					{
						lstFex[i][p][1] = F(myFPs[p].forceType, w, myFPs[p].Amp, myFPs[p].k, tArr[i]);
					}
					if (myFPs[p].myforcePlaces.z == true)
					{
						lstFex[i][p][2] = F(myFPs[p].forceType, w, myFPs[p].Amp, myFPs[p].k, tArr[i]);
					}
				}
			}
		};
	};
	int savelarge()
	{
		String^ path = L"F:\\MyTestFile0.txt";

		try
		{
			if (File::Exists(path))
			{
				File::Delete(path);
			}
			StreamWriter^ sw = gcnew StreamWriter(path);
			sw->WriteLine("Thiqwsqs");
			sw->WriteLine("is some text");
			sw->WriteLine("written to test");
			sw->WriteLine("for reading...");
			sw->Close();
		}
		catch (Exception^ e)
		{
			Console::WriteLine("The process failed: {0}", e->ToString());
			return 1;
		}
		return 0;
	};
}