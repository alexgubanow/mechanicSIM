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
	static long long elastic = 215000000000;

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
		Derivatives(int demesia)
		{
			coord = gcnew array<double>(demesia);
			force = gcnew array<double>(demesia);
			displ = gcnew array<double>(demesia);
			velos = gcnew array<double>(demesia);
			accl = gcnew array<double>(demesia);
			jerk = gcnew array<double>(demesia);
		}
	};

	public ref class Point
	{
	public:
		int index;
		Derivatives ^ derivatives;
		Point(int newIndex, int demesia)
		{
			index = newIndex;
			derivatives = gcnew Derivatives(demesia);
		}
	};

	public ref class Element
	{
	public:
		int index;
		int point1;
		int point2;
		Element(int ElementIndex, int point1Index, int point2Index, int demesia)
		{
			index = ElementIndex;
			point1 = point1Index;
			point2 = point2Index;
		}
	};
	public ref class Node
	{
	public:
		nodeFreedoms nodeFreedom;
		array<int>^ ListOfLinks;
		array<int>^ ListOfPoints;
		Derivatives^ derivatives;
		Node(nodeFreedoms newnodeFreedom, array<int>^ newListOfLinks, array<int>^ newListOfPoints, int demesia)
		{
			nodeFreedom = newnodeFreedom;
			ListOfLinks = newListOfLinks;
			ListOfPoints = newListOfPoints;
			derivatives = gcnew Derivatives(demesia);
		}
	};

	public ref class timeMoment
	{
	public:
		array<Node^>^ Nodes;
		array<Element^>^ Elements;
		array<Point^>^ Points;
		timeMoment(int numNodes, int numElements, int demesia)
		{
			Nodes = gcnew array<Node^>(numNodes);
			Elements = gcnew array<Element^>(numElements);
			Nodes[0] = gcnew Node(nodeFreedoms::x, gcnew array<int>(1) { 0 }, gcnew array<int>(1) { 0 }, demesia);
			for (int i = 1, j = 1, k = 0; i < numNodes - 1; i++, j += 2, k++)
			{
				Elements[k] = gcnew Element(k, j - 1, j, demesia);
				Nodes[i] = gcnew Node(nodeFreedoms::x, gcnew array<int>(2) { i - 1, i }, gcnew array<int>(2) { j, j + 1 }, demesia);
			}
			Nodes[numNodes - 1] = gcnew Node(nodeFreedoms::x, gcnew array<int>(1) { numNodes - 2 }, gcnew array<int>(2) { numNodes }, demesia);
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
					/*timeMoments[momentNow]->Elements[0]->Point1->derivatives->accl[0] = 0;
					timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1->derivatives->force[0] = ((elastic * A / _l) *
						(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2->derivatives->displ[0] -
							timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1->derivatives->displ[0]));

					timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2->derivatives->force[0] = 0 - ((elastic * A / _l) *
						(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2->derivatives->displ[0] -
							timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1->derivatives->displ[0]));

					euler(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point1->derivatives, time[1], _m, _l);
					euler(timeMoments[momentNow]->Elements[timeMoments[momentNow]->Nodes[i]->ListOfLinks[j]]->Point2->derivatives, time[1], _m, _l);*/
				}
				//sum all
			}
			/*timeMoments[momentNow]->calcNodesMove();*/
		}
		void euler(Derivatives^ %moment, double deltat, double massa, double l)
		{
			if (moment->force[0] != 0)
			{
				moment->accl[0] = moment->force[0] / massa;
				moment->velos[0] = moment->accl[0] * deltat;
				moment->displ[0] = moment->velos[0] * deltat;
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
		LinearModel(int counts, double dt, int numNodes, int numElements, double massa, double lenght, double b, double h)
		{
			_m = massa;
			_l = lenght;
			_b = b;
			_h = h;
			_counts = counts;
			_dt = dt;
			initTime();
			initTimeMoments(numNodes, numElements);
		}
		void initTime()
		{
			time = gcnew array<double>(_counts);
			for (int i = 1; i < _counts; i++)
			{
				time[i] = time[i - 1] + _dt;
			}
		}
		void initTimeMoments(int numNodes, int numElements)
		{
			timeMoments = gcnew array<timeMoment^>(_counts);
			for (int i = 0; i < _counts; i++)
			{
				timeMoments[i] = gcnew timeMoment(numNodes, numElements, 3);
			}
		}
		void applyLoad(int freq, double amp)
		{
			for (int i = 0; i < timeMoments->Length; i++)
			{
				timeMoments[i]->Nodes[0]->derivatives->displ[0] = amp * sin(time[i] * 2 * M_PI * freq);
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