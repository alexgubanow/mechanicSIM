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

	public ref class coords
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};
	public ref class forces
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};
	public ref class displs
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};
	public ref class veloss
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};
	public ref class accls
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};
	public ref class jerks
	{
	private:
		array<double> ^x;
	public:
		property double default[int]
		{
			double get(int index)
		{
			return x[index];
		}
		void set(int index, double value)
		{
			x[index] = value;
		}
		}
	};

	ref class Point
	{
	private:
		static int _index = 0;
		static coords _coord;
		static array<double^>^ _force;
		static array<double^>^ _displ;
		static array<double^>^ _velos;
		static array<double^>^ _accl;// 3-th derivative
		static array<double^>^ _jerk;// 4-th derivative
	public:
		property int index { int get() { return _index; } void set(int value) { _index = value; } };
		property coords coord { coords get() { return _coord; } void set(coords value) { _coord = value; } };
		property array<double^>^ force { array<double^>^ get() { return _force; } void set(array<double^>^ value) { _force = value; } };
		property array<double^>^ displ { array<double^>^ get() { return _displ; } void set(array<double^>^ value) { _displ = value; } };
		property array<double^>^ velos { array<double^>^ get() { return _velos; } void set(array<double^>^ value) { _velos = value; } };
		property array<double^>^ accl { array<double^>^ get() { return _accl; } void set(array<double^>^ value) { _accl = value; } };
		property array<double^>^ jerk { array<double^>^ get() { return _jerk; } void set(array<double^>^ value) { _jerk = value; } };
		void initValues(int, array<double^>^, array<double^>^, array<double^>^, array<double^>^, array<double^>^, array<double^>^);
	};
	void Point::initValues(int newindex, array<double^>^ newcoord, array<double^>^ newforce, array<double^>^ newdispl, array<double^>^ newvelos, array<double^>^ newaccl, array<double^>^ newjerk)
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
	private:
		static nodeFreedoms _nodeFreedom;
		static array<int^>^ _members;
		static array<double^>^ _coord;
		static array<double^>^ _force;
		static array<double^>^ _displ;
		static array<double^>^ _velos;
		static array<double^>^ _accl;// 3-th derivative
		static array<double^>^ _jerk;// 4-th derivative
	public:
		Node() {};
		property nodeFreedoms nodeFreedom { nodeFreedoms get() { return _nodeFreedom; } void set(nodeFreedoms value) { _nodeFreedom = value; } };
		property array<int^>^ members { array<int^>^ get() { return _members; } void set(array<int^>^ value) { _members = value; } };
		property array<double^>^ coord { array<double^>^ get() { return _coord; } void set(array<double^>^ value) { _coord = value; } };
		property array<double^>^ force { array<double^>^ get() { return _force; } void set(array<double^>^ value) { _force = value; } };
		property array<double^>^ displ { array<double^>^ get() { return _displ; } void set(array<double^>^ value) { _displ = value; } };
		property array<double^>^ velos { array<double^>^ get() { return _velos; } void set(array<double^>^ value) { _velos = value; } };
		property array<double^>^ accl { array<double^>^ get() { return _accl; } void set(array<double^>^ value) { _accl = value; } };
		property array<double^>^ jerk { array<double^>^ get() { return _jerk; } void set(array<double^>^ value) { _jerk = value; } };
		void initValues(nodeFreedoms newnodeFreedom, array<int^>^ newmembers, array<double^>^ newcoord, array<double^>^ newforce, array<double^>^ newdispl, array<double^>^ newvelos, array<double^>^ newaccl, array<double^>^ newjerk)
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
	private:
		static array<Point^>^ _Points;
		static array<Node^>^ _Nodes;
	public:
		timeMoment() {};
		property array<Point^>^ Points { array<Point^>^ get() { return _Points; } void set(array<Point^>^ value) { _Points = value; } };
		property array<Node^>^ Nodes { array<Node^>^ get() { return _Nodes; } void set(array<Node^>^ value) { _Nodes = value; } };
		void initPoints(int numPoints)
		{
			Points = gcnew array<Point^>(numPoints);
			for (int i = 0; i < numPoints; i++)
			{
				Points[i]->initValues(i, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 });
			}
		}
		void initNodes(int numNodes)
		{
			Nodes = gcnew array<Node^>(numNodes);
			Nodes[0]->initValues(nodeFreedoms::x, gcnew array<int^>(2) { 0, 0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 });
			for (int i = 1; i < numNodes - 1; i++)
			{
				Nodes[i]->initValues(nodeFreedoms::x, gcnew array<int^>(2) { i, i + 1 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 });
			}
			Nodes[numNodes - 1]->initValues(nodeFreedoms::x, gcnew array<int^>(2) { numNodes - 1, numNodes - 1 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 }, gcnew array<double^>(3) { 0.0, 0.0, 0.0 });
		}
		void calcNodesMove()
		{
			for (int i = 0; i < Nodes->Length; i++)
			{
				//Nodes[i]->force = Points[Nodes[i]->members[0]]->force;
				//Nodes[i]->force = Points[Nodes[i]->members[0]]->force + Points[Nodes[i]->members[1]]->force;
			}
		}
	};

	public ref class LinearModel
	{
	private:
		static array<timeMoment^>^ _timeMoments;
		void calcOneMove(int momentNow, int prevMoment);
	public:
		property array<timeMoment^>^ timeMoments { array<timeMoment^>^ get() { return _timeMoments; } void set(array<timeMoment^>^ value) { _timeMoments = value; } };
		void initTimeMoments(int counts, int numNodes, int numPoints);
		void calcMove();
	};
	void LinearModel::initTimeMoments(int counts, int numNodes, int numPoints)
	{
		timeMoments = gcnew array<timeMoment^>(counts);
		for (int i = 0; i < counts; i++)
		{
			timeMoments[i]->initPoints(numPoints);
			timeMoments[i]->initNodes(numNodes);
		}
	}
	void LinearModel::calcOneMove(int momentNow, int prevMoment)
	{
		/*for (int i = 0; i < timeMoments[momentNow]->Points->Length; i++)
		{
			timeMoments[momentNow]->Points[i]->force = gcnew array<double>(3){ (double)i,0,0 };
			timeMoments[momentNow]->calcNodesMove();
		}*/
	}
	void LinearModel::calcMove()
	{
		for (int i = 1; i < 0; i++)
		{
			calcOneMove(i, i - 1);
		}
	}
}