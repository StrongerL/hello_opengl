#include "HalfEdgeDataStructure.h"
#include "EulerOperation.h"
#include "Sweep.h"

#include <vector>

using namespace std;

Loop* Sweep::opposite_loop(Loop* loop) {
	HalfEdge* he = loop->ledge;
	Edge* edg = he->edg;
	return edg->he1 == he ? edg->he2->wloop : edg->he1->wloop;
}

Solid* Sweep::sweep(Face* bottom_face, double x, double y, double z) {
	Solid* solid = bottom_face->fsolid;
	vector<Loop*> loops = bottom_face->floops;
	Face* up_face = NULL;

	for (int i = 0; i < loops.size(); i++) {
		Loop* loop = opposite_loop(loops[i]);
		Face* face = loop->lface;
		HalfEdge* he = loop->ledge;
		Vertex* src = he->startv;
		Vertex* srcup = new Vertex(src->x + x, src->y + y, src->z + z);
		EulerOperation::mev(src, srcup, loop);
		Vertex* prvup = srcup;
		he = he->nxt;
		Vertex* vtx = he->startv;
		while (vtx != src) {
			Vertex* up = new Vertex(vtx->x + x, vtx->y + y, vtx->z + z);
			EulerOperation::mev(vtx, up, loop);
			EulerOperation::mef(up, prvup, face);
			he = he->nxt;
			vtx = he->startv;
			prvup = up;
		}
		EulerOperation::mef(srcup, prvup, face);
		if (i != 0) {
			EulerOperation::kfmrh(up_face, face);
		} else {
			up_face = face;
		}
	}
	
	return solid;
}
