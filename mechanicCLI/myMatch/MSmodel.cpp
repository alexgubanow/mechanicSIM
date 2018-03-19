#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <typeinfo>
#using <system.dll>
#using <mscorlib.dll>
using namespace std;
using namespace System;
using namespace System::IO;

namespace myMatch
{
	public enum class nodeFreedoms { x, y, z, xy, xz, yz, xyz };

	public ref class Point
	{
	public:
		int index = 0;
		array<double>^ coord;
		array<double>^ force;
		array<double>^ displ;
		array<double>^ velos;
		array<double>^ accl;// 3-th derivative
		array<double>^ jerk;// 4-th derivative
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
		array<double>^ accl;// 3-th derivative
		array<double>^ jerk;// 4-th derivative
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
				Nodes[i]->force[0] = Points[(int)(Nodes[i]->members[0])]->force[0];
				//Nodes[i]->force = Points[Nodes[i]->members[0]]->force + Points[Nodes[i]->members[1]]->force;
			}
		}
	};

	public ref class LinearModel
	{
	private:
		void calcOneMove(int momentNow, int prevMoment)
		{
			for (int i = 0; i < timeMoments[momentNow]->Points->Length; i++)
			{
				//timeMoments[momentNow]->Points[i]->force[0] = (double)i;
				timeMoments[momentNow]->calcNodesMove();
			}
		};
	public:
		array<timeMoment^>^ timeMoments;
		array<double>^ time;
		void initTime(int counts, double dt)
		{
			time = gcnew array<double>(counts);
			for (int i = 1; i < counts; i++)
			{
				time[i] = (double)time[i - 1] + dt;
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
		};
		void applyLoad()
		{
			for (int i = 0; i < timeMoments->Length; i++)
			{
				for (size_t j = 0; j < timeMoments[i]->Points->Length; j++)
				{
					timeMoments[i]->Points[j]->force[0] = (double)sin(time[i] * 100);
					timeMoments[i]->calcNodesMove();
				}
			}
		};
		void calcMove()
		{
			for (int i = 1; i < timeMoments->Length; i++)
			{
				calcOneMove(i, i - 1);
			}
		};
	};
}