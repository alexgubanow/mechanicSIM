#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <typeinfo>
#using <system.dll>
#using <mscorlib.dll>

#include "integrSchems.h"

using namespace std;
using namespace System;
using namespace System::IO;

namespace myMatch
{
	static int elastic = 215000000000;

	public enum class nodeFreedoms { x, y, z, xy, xz, yz, xyz };
	//derivatives
	public ref class Derivatives
	{
	public:
		array<double>^ coord;
		array<double>^ force;
		array<double>^ displ;
		array<double>^ velos;
		array<double>^ accl;
		array<double>^ jerk;
		void initValues(array<double>^, array<double>^, array<double>^, array<double>^, array<double>^, array<double>^);
	};
	void Derivatives::initValues(array<double>^ newcoord, array<double>^ newforce, array<double>^ newdispl, array<double>^ newvelos, array<double>^ newaccl, array<double>^ newjerk)
	{
		coord = newcoord;
		force = newforce;
		displ = newdispl;
		velos = newvelos;
		accl = newaccl;
		jerk = newjerk;
	}

	public ref class Point
	{
	public:
		Derivatives derivatives;
		void initValues(Derivatives newDeriv)
		{
			derivatives.initValues(newDeriv.coord, newDeriv.force, newDeriv.displ, newDeriv.velos, newDeriv.accl, newDeriv.jerk);
		}
	};

	public ref class Element
	{
	public:
		int index;
		Point Point1;
		Point Point2;
		void initValues(int newIndex, Derivatives newValue)
		{
			index = newIndex;
			//Point1.initValues(newValue);
		}
	};
	public ref class Node
	{
	public:
		nodeFreedoms nodeFreedom;
		array<int>^ ListOfLinks;
		array<int>^ ListOfPoints;
		Derivatives derivatives;
		void initValues(nodeFreedoms newnodeFreedom, array<int>^ newListOfLinks, array<int>^ newListOfPoints, Derivatives newDeriv)
		{
			nodeFreedom = newnodeFreedom;
			ListOfLinks = newListOfLinks;
			ListOfPoints = newListOfPoints;
			derivatives.initValues(newDeriv.coord, newDeriv.force, newDeriv.displ, newDeriv.velos, newDeriv.accl, newDeriv.jerk);
		}
	};

	public ref class timeMoment
	{
	public:
		array<Node^>^ Nodes;
		array<Element^>^ Elements;
		void initPoints(int numPoints)
		{
			Elements = gcnew array<Element^>(numPoints);
			for (int i = 0; i < numPoints; i++)
			{
				Elements[i] = gcnew Element;
				Elements[i]->initValues(i, gcnew Derivatives(gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 } ));
			}
		}
		void initNodes(int numNodes)
		{
			Nodes = gcnew array<Node^>(numNodes);
			Nodes[0] = gcnew Node;
			Nodes[0]->initValues(nodeFreedoms::x, gcnew array<int>(2) { 0, 0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 });
			for (int i = 1; i < numNodes - 1; i++)
			{
				Nodes[i] = gcnew Node;
				Nodes[i]->initValues(nodeFreedoms::x, gcnew array<int>(2) { i, i + 1 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 });
			}
			Nodes[numNodes - 1] = gcnew Node;
			Nodes[numNodes - 1]->initValues(nodeFreedoms::x, gcnew array<int>(2) { numNodes - 1, numNodes - 1 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 });
		}
	};
	public ref class LinearModel
	{
		void calcOneMove(int momentNow, int prevMoment)
		{
			double A = _b * _h;
			//overview links of curr node
			for (int i = 0; i < timeMoments[momentNow]->Nodes->Length; i++)
			{
				//calc each link
				for (int j = 0; j < timeMoments[momentNow]->Nodes[i]->ListOfLinks->Length; j++)
				{
					timeMoments[momentNow]->Elements[0]->Point1.derivatives.accl[0] = 0;
					timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1.derivatives.force[0] = ((elastic * A / _l) *
						(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2.derivatives.displ[0] -
							timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1.derivatives.displ[0]));

					timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2.derivatives.force[0] = 0 - ((elastic * A / _l) *
						(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2.derivatives.displ[0] -
							timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1.derivatives.displ[0]));

					euler(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1.derivatives, time[1], _m, _l);
					euler(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2.derivatives, time[1], _m, _l);
				}
				//sum all
			}






			/*for (int i = 0; i < timeMoments[momentNow]->Points->Length; i += 2)
			{
				double A = _b * _h;				
				if(i != 0 && i != timeMoments[momentNow]->Points->Length)
				{
					timeMoments[momentNow]->Points[i]->force[0] = (((elastic * A) / _l) * ((timeMoments[prevMoment]->Points[i + 1]->displ[0] - timeMoments[prevMoment]->Points[i]->displ[0])));
				}
				timeMoments[momentNow]->Points[i + 1]->force[0] = 0 - (((elastic * A) / _l) * ((timeMoments[prevMoment]->Points[i + 1]->displ[0] - timeMoments[prevMoment]->Points[i]->displ[0])));
				euler(timeMoments[momentNow]->Points[i], time[1], _m, _l);
				euler(timeMoments[momentNow]->Points[i + 1], time[1], _m, _l);
			}
			timeMoments[momentNow]->calcNodesMove();*/
		}
		void euler(Derivatives %momentNow, double deltat, double massa, double l)
		{
			if (momentNow.force[0] != 0)
			{
				momentNow.accl[0] = momentNow.force[0] / massa;
				momentNow.velos[0] = momentNow.accl[0] * deltat;
				momentNow.displ[0] = momentNow.velos[0] * deltat;
			}
			//ap[2] = F[2] / massa * l * l * 0.5;
		};
	public:
		array<timeMoment^>^ timeMoments;
		array<double>^ time;
		double _m;
		double _l;
		double _b;
		double _h;
		int _counts;
		double _dt;
		LinearModel(int counts, double dt, int numNodes, int numPoints, double massa, double lenght, double b, double h)
		{
			_m = massa;
			_l = lenght;
			_b = b;
			_h = h;
			_counts = counts;
			_dt = dt;
			initTime();
			initTimeMoments(numNodes, numPoints);
		}
		void initTime()
		{
			time = gcnew array<double>(_counts);
			for (int i = 1; i < _counts; i++)
			{
				time[i] = time[i - 1] + _dt;
			}
		}
		void initTimeMoments(int numNodes, int numPoints)
		{
			timeMoments = gcnew array<timeMoment^>(_counts);
			for (int i = 0; i < _counts; i++)
			{
				timeMoments[i] = gcnew timeMoment;
				timeMoments[i]->initPoints(numPoints);
				timeMoments[i]->initNodes(numNodes);
			}
		}
		void applyLoad(int freq, double amp)
		{
			for (int i = 0; i < timeMoments->Length; i++)
			{
				timeMoments[i]->Nodes[0]->derivatives.displ[0] = amp * sin(time[i] * 2 * M_PI * freq);
			}
		}
		void calcMove()
		{
			for (int i = 1; i < timeMoments->Length; i++)
			{
				calcOneMove(i, i - 1);
			}
		}
	};
}