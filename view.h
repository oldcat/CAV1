#ifndef _rt_H
#define _rt_H

#include <cmath>
#include <map>
#include <vector>
#include <cstring>
#include <GL/glut.h>
#include "matrix4f.h"


using namespace std;

//
// Sample code for physics simulation
//


// Implements cloth simulation


class Vector3f;
class Triangle;
class TriangleMesh;
class Edge;





class Triangle {
friend class TriangleMesh;

	int _id;
	int _vertex[3];
	int _normal[3];
	int _edge[3];
	float _min, _max;
	float _color; 
	float _area;
	float _ratio;

public:

	Triangle(int v1, int v2, int v3, int n1, int n2, int n3) 
	{
		_vertex[0] = v1;  _vertex[1] = v2;  _vertex[2] = v3;  
		_normal[0] = n1;  _normal[1] = n2;  _normal[2] = n3;  
		
	};

	void setMorseMinMax(float min, float max)
	{
		_min = min; _max = max;
	}

	void getMorseMinMax(float & min, float & max)
	{
		min = _min; max = _max;
	}

	void setEdge(int e1, int e2, int e3)
	{
		_edge[0] = e1;
		_edge[1] = e2;
		_edge[2] = e3;
	};

	int edge(int i) { return _edge[i];};
	int id() { return _id;};
	void setColor(float f) { _color = f ;};
	float color() { return _color;};
};

class Edge {

	friend bool contain(Edge & e, map < pair <int, int> , Edge > & list) ;
	friend int edgeID(Edge & e, map < pair <int, int> , Edge > & list) ;
	friend bool contain(Edge & e, vector < Edge > & list) ;
	friend int edgeID(Edge & e, vector < Edge > & list) ;

	int _v1,_v2;
	vector <int> _trig_list;
	float length;
	int _id;

public: 

	Edge () {};

	Edge (int i, int j)
	{
		_v1 = i; _v2 = j;
	};

	bool operator == (Edge & e)
	{
		if (this->_v1 == e._v1 && this->_v2 == e._v2) return true;
		if (this->_v1 == e._v2 && this->_v2 == e._v1) return true;

		return false;
	}

	void add_triangle(int trig)
	{
		for (int i = 0; i < _trig_list.size(); i++) 
			if (trig == _trig_list[i]) return;

		_trig_list.push_back(trig);
	}

	void other_trig(const int trig, vector <int> & others) 
	{
		for (int i = 0; i < _trig_list.size(); i++) {
			if (_trig_list[i] == trig) continue; 
			else others.push_back(_trig_list[i]);
		}
	}


	void setId(int id) { _id = id;};
	int id() { return _id;};
	int v1() { return _v1;};
	int v2() { return _v2;};
	vector <int> getTrigList() { return _trig_list ;};
};




struct Node {
	int _id;

	vector<int> edges_to;     
	vector<float> edges_cost;   

	bool done;   
	float cost;    
};






float fmax(float f1,float f2, float f3) {
	float f = f1;

	if (f < f2) f = f2;
	if (f < f3) f = f3;

	return f;
};

float fmin(float f1,float f2, float f3) {
	float f = f1;

	if (f > f2) f = f2;
	if (f > f3) f = f3;

	return f;
};


class TriangleMesh 
{
	vector <Vector3f> _v;
	vector <Vector3f> _vn;
	vector <Triangle> _trig;
	vector <Node> _node;
	vector <Edge> _edge;

//	map <pair < int, int > , Edge > _edge;

	float _xmax, _xmin, _ymax, _ymin, _zmin, _zmax;



public: 
	TriangleMesh(char * filename) { loadFile(filename) ;};
	TriangleMesh() {};
	void loadFile(char * filename);

	int trigNum() { return _trig.size() ;};

	void getTriangleVertices(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3)
	{
		v1 = _v[_trig[i]._vertex[0]]; 
		v2 = _v[_trig[i]._vertex[1]]; 
		v3 = _v[_trig[i]._vertex[2]]; 
	}
	
	void getTriangleNormals(int i, Vector3f & v1, Vector3f & v2, Vector3f & v3)
	{
		v1 = _vn[_trig[i]._normal[0]]; 
		v2 = _vn[_trig[i]._normal[1]]; 
		v3 = _vn[_trig[i]._normal[2]]; 
	}
		

	void getMorseValue(int i, float & v1, float & v2, float & v3)
	{
		v1 = _node[_trig[i]._vertex[0]].cost; 
		v2 = _node[_trig[i]._vertex[1]].cost; 
		v3 = _node[_trig[i]._vertex[2]].cost; 
	}

	float color(int i) { return _trig[i].color();};


	void setMorseMinMax(int i, float min, float max)
	{
		_trig[i].setMorseMinMax(min,max);
	}

	void getMorseMinMax(int i, float & min, float & max)
	{
		_trig[i].getMorseMinMax(min,max);
	}


	void calcTriangleArea() 
	{
		Vector3f v1,v2,v3;

		for (int i = 0 ;i < _trig.size(); i++) 
		{
			getTriangleVertices(i, v1,v2,v3);
			v3 -= v1;
			v2 -= v1;

			_trig[i]._area = 0.5f*sqrt(v3.dot(v3)*v2.dot(v2) - (v3.dot(v2)*(v3.dot(v2))));  
//			cout << "trig " << i << " v2 " << v2 << " v3 " << v3 << " area = " << _trig[i]._area << endl;
		}
	}
};

#endif //_rt_H
