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
			for (int i = 0; i < Nodes->Length; i++)
			{
				Nodes[i]->force[0] = Points[Nodes[i]->members[0]]->force[0] + Points[Nodes[i]->members[1]]->force[0];
				Nodes[i]->accl[0] = Points[Nodes[i]->members[0]]->accl[0] + Points[Nodes[i]->members[1]]->accl[0];
				Nodes[i]->velos[0] = Points[Nodes[i]->members[0]]->velos[0] + Points[Nodes[i]->members[1]]->velos[0];
				Nodes[i]->displ[0] = Points[Nodes[i]->members[0]]->displ[0] + Points[Nodes[i]->members[1]]->displ[0];
			}
		}
	};
	public ref class LinearModel
	{
		void calcOneMove(int momentNow, int prevMoment)
		{
			for (int i = 0; i < timeMoments[momentNow]->Points->Length; i+=2)
			{
				//calc force
				//old version
				//F[0] = ((elastic * A) / l) * (-(currxi[0] - lastxi[0]));
				double A = _b * _h;
				timeMoments[momentNow]->Points[i + 1]->force[0] = -((elastic * A) / _l) * ((timeMoments[prevMoment]->Points[i + 1]->displ[0] - timeMoments[prevMoment]->Points[i]->displ[0]));
				timeMoments[momentNow]->Points[i]->force[0] = - timeMoments[momentNow]->Points[i + 1]->force[0];

				//integrate it
				timeMoments[momentNow]->Points[i]->accl[0] = timeMoments[momentNow]->Points[i]->force[0] / _m;
				timeMoments[momentNow]->Points[i]->velos[0] = (timeMoments[prevMoment]->Points[i]->accl[0] * time[1]);
				if(i != 0)
				{
					timeMoments[momentNow]->Points[i]->displ[0] = (timeMoments[prevMoment]->Points[i]->velos[0] * time[1]);
				}
				timeMoments[momentNow]->Points[i + 1]->accl[0] = timeMoments[momentNow]->Points[i + 1]->force[0] / _m;
				timeMoments[momentNow]->Points[i + 1]->velos[0] = (timeMoments[prevMoment]->Points[i + 1]->accl[0] * time[1]);
				timeMoments[momentNow]->Points[i + 1]->displ[0] = (timeMoments[prevMoment]->Points[i + 1]->velos[0] * time[1]);

				/*integrSchems::euler(
					timeMoments[prevMoment]->Points[i]->displ, timeMoments[prevMoment]->Points[i]->velos, timeMoments[prevMoment]->Points[i]->accl, timeMoments[prevMoment]->Points[i]->force,
					time[1], _m, _l,
					timeMoments[momentNow]->Points[i]->displ, timeMoments[momentNow]->Points[i]->velos, timeMoments[momentNow]->Points[i]->accl);*/
			}
			timeMoments[momentNow]->calcNodesMove();
		}
	public:
		array<timeMoment^>^ timeMoments;
		array<double>^ time;
		double _m;
		double _l;
		double _b;
		double _h;
		LinearModel(int counts, double dt, int numNodes, int numPoints, double massa, double lenght, double b, double h)
		{
			_m = massa;
			_l = lenght;
			_b = b;
			_h = h;
			initTime(counts, dt);
			initTimeMoments(counts, numNodes, numPoints);
		}
		void initTime(int counts, double dt)
		{
			time = gcnew array<double>(counts);
			for (int i = 1; i < counts; i++)
			{
				time[i] = time[i - 1] + dt;
			}
		}
		void initTimeMoments(int counts, int numNodes, int numPoints)
		{
			timeMoments = gcnew array<timeMoment^>(counts);
			for (int i = 0; i < counts; i++)
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
				timeMoments[i]->Points[0]->displ[0] = amp * sin( time[i] * 2 * M_PI * freq);
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