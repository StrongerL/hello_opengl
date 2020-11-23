#include "Draw.h"
#include "HalfEdgeDataStructure.h"

#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void Draw::print_coordinate(Solid* solid) {
	vector<Face*> faces = solid->sfaces;
	for (int i = 0; i < faces.size(); i++) {
		cout << "face[" << i << "]" << endl;
		vector<Loop*> loops = faces[i]->floops;
		for (int j = 0; j < loops.size(); j++) {
			cout << "face[" << i << "] -> " << "loop[" << j << "]" << endl;
			Loop* loop = loops[j];
			HalfEdge* he = loop->ledge;
			HalfEdge* src_edge = he;
			cout << "[" << he->startv->x << ", " << he->startv->y << ", " << he->startv->z << "]  ";
			he = he->nxt;
			while (he != src_edge) {
				cout << "[" << he->startv->x << ", " << he->startv->y << ", " << he->startv->z << "]  ";
				he = he->nxt;
			}
			cout << endl;
		}
	}
}

void Draw::draw(Solid* solid) {
	

}
