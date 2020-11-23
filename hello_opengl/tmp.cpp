/*
#include <iostream>
#include <fstream>

#include "HalfEdgeDataStructure.h"
#include "EulerOperation.h"
#include "Sweep.h"
#include "Draw.h"

using namespace std;

Face* build_face() {
	fstream in;
	in.open("in.txt");
	int loop_cnt;
	in >> loop_cnt;
	// �⻷
	int vtx_cnt;
	in >> vtx_cnt;
	double x, y, z;
	in >> x >> y >> z;
	Solid* solid = EulerOperation::mvsf(x, y, z);
	Face* face = solid->sfaces[0];
	Loop* loop = face->floops[0];
	Vertex* prvv = solid->sorigin;
	for (int i = 1; i < vtx_cnt; i++) {
		in >> x >> y >> z;
		Vertex* newv = new Vertex(x, y, z);
		EulerOperation::mev(prvv, newv, loop);
		prvv = newv;

	}
	EulerOperation::mef(prvv, solid->sorigin, face);
	// �ڻ�
	for (int i = 1; i < loop_cnt; i++) {
		in >> vtx_cnt;
		prvv = solid->sorigin;
		in >> x >> y >> z;
		Vertex* newv = new Vertex(x, y, z);
		Edge* edge = EulerOperation::mev(prvv, newv, loop);
		prvv = edge->he1->endv;
		for (int j = 1; j < vtx_cnt; j++) {
			in >> x >> y >> z;
			Vertex* newv = new Vertex(x, y, z);
			EulerOperation::mev(prvv, newv, loop);
			prvv = newv;
		}
		EulerOperation::mef(prvv, edge->he1->endv, face);
		EulerOperation::kemr(edge);
	}
	return face;
}

int main() {
	// �������
	Face* face = build_face();
	// ɨ��
	Solid* solid = Sweep::sweep(face, 0.0, 0.0, 10.0);
	// ��ӡ����
	Draw::print_coordinate(solid);
	// ����
	Draw::draw(solid);
}
*/
