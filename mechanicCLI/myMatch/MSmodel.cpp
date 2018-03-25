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

	public ref class Point
	{
	public:
		int index;
		array<double>^ coord;
		array<double>^ force;
		array<double>^ displ;
		array<double>^ velos;
		array<double>^ accl;
		array<double>^ jerk;
		void initValues(int, array<double>^, array<double>^, array<double>^, array<double>^, array<double>^, array<double>^);
	};
	void Point::initValues(int newindex, array<double>^ newcoord, array<double>^ newforce, array<double>^ newdispl, array<double>^ newvelos, array<double>^ newaccl, array<double>^ newjerk)
	{
		index = newindex;
		coord = newcoord;
		force = newforce;
		displ = newdispl;
		velos = newvelos;
		accl = newaccl;
		jerk = newjerk;
	}

	public ref class Node
	{
	public:
		nodeFreedoms nodeFreedom;
		array<int>^ members;
		array<double>^ coord;
		array<double>^ force;
		array<double>^ displ;
		array<double>^ velos;
		array<double>^ accl;
		array<double>^ jerk;
		void initValues(nodeFreedoms newnodeFreedom, array<int>^ newmembers, array<double>^ newcoord, array<double>^ newforce, array<double>^ newdispl, array<double>^ newvelos, array<double>^ newaccl, array<double>^ newjerk)
		{
			nodeFreedom = newnodeFreedom;
			members = newmembers;
			coord = newcoord;
			force = newforce;
			displ = newdispl;
			velos = newvelos;
			accl = newaccl;
			jerk = newjerk;
		}
	};

	public ref class timeMoment
	{
	public:
		array<Point^>^ Points;
		array<Node^>^ Nodes;
		void initPoints(int numPoints)
		{
			Points = gcnew array<Point^>(numPoints);
			for (int i = 0; i < numPoints; i++)
			{
				Points[i] = gcnew Point;
				Points[i]->initValues(i, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 }, gcnew array<double>(3) { 0.0, 0.0, 0.0 });
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
		void calcNodesMove()
		{
			Nodes[0]->force[0] = Points[0]->force[0];
			Nodes[0]->accl[0] = Points[0]->accl[0];
			Nodes[0]->velos[0] = Points[0]->velos[0];
			Nodes[0]->displ[0] = Points[0]->displ[0];

			for (int i = 1; i < Nodes->Length - 1; i++)
			{
				Nodes[i]->force[0] = Points[Nodes[i]->members[0]]->force[0] + Points[Nodes[i]->members[1]]->force[0];
				Nodes[i]->accl[0] = Points[Nodes[i]->members[0]]->accl[0] + Points[Nodes[i]->members[1]]->accl[0];
				Nodes[i]->velos[0] = Points[Nodes[i]->members[0]]->velos[0] + Points[Nodes[i]->members[1]]->velos[0];
				Nodes[i]->displ[0] = Points[Nodes[i]->members[0]]->displ[0] + Points[Nodes[i]->members[1]]->displ[0];
				if (i != Nodes->Length - 1)
				{
					Points[Nodes[i]->members[1]]->displ[0] = Points[Nodes[i]->members[0]]->displ[0];
				}
			}
		}
	};
	public ref class LinearModel
	{
		void calcOneMove(int momentNow, int prevMoment)
		{
			for (int i = 0; i < timeMoments[momentNow]->Points->Length; i += 2)
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
			timeMoments[momentNow]->calcNodesMove();
		}
		void euler(Point^ %momentNow, double deltat, double massa, double l)
		{
			if (momentNow->force[0] != 0)
			{
				momentNow->accl[0] = momentNow->force[0] / massa;
				momentNow->velos[0] = momentNow->accl[0] * deltat;
				momentNow->displ[0] = momentNow->velos[0] * deltat;
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
				timeMoments[i]->Points[0]->displ[0] = amp * sin(time[i] * 2 * M_PI * freq);
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