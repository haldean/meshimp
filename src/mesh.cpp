#include "mesh.h"

vertex::vertex() {
  loc.setZero();
  normal.setZero();
}

vertex::vertex(const vertex &other) {
  loc = other.loc;
  normal = other.normal;
}

face::face() {
  verts = vector<vertex*>(3);
}

face::face(const face &other) {
  verts = other.verts;
  neighbors = other.neighbors;
}

mesh::mesh() {
  faces.reserve(1000);
}
