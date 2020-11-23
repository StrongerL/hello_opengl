#pragma once

#include "HalfEdgeDataStructure.h"
using namespace std;

class EulerOperation {

private:

public:
	static Solid* mvsf(double x, double y, double z);
	static Edge* mev(Vertex* startv, Vertex* newv, Loop* loop);
	static Face* mef(Vertex* startv, Vertex* endv, Face* face);
	static Loop* kemr(Edge* edge);
	static void kfmrh(Face* bigf, Face* smallf);
};
