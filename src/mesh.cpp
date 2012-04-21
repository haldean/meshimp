#include "mesh.h"

vertex::vertex() {
  loc.setZero();
  normal.setZero();
}

void vertexCopy() {
  cout << "vertex copied" << endl;
}

vertex::vertex(const vertex &other) {
  vertexCopy();
  loc = other.loc;
  normal = other.normal;
}

bool vertex::operator==(const vertex &other) {
  return loc == other.loc;
}

face::face() {
  verts = vector<vertex*>(3);
}

face::face(const face &other) {
  verts = other.verts;
  neighbors = other.neighbors;
}

float face::area() const {
  Vector3f e1 = verts[0]->loc - verts[1]->loc,
           e2 = verts[0]->loc - verts[2]->loc;
  return e1.cross(e2).norm();
}

mesh::mesh() {
  faces.reserve(1000);
  manifold = false;
}

void mesh::calculateNormals(normal_mode mode) {
  for (auto it = faces.begin(); it != faces.end(); it++) {
    face *f = *it;
    for (auto vit = f->verts.begin(); vit != f->verts.end(); vit++) {
      if (mode == NO_NORMALS) {
        (*vit)->normal.setZero();
      } else {
        vertex *v = *vit;
        if (!v->normal.isZero()) continue;

        vector<face*> neighbors;
        face* loopf = f;
        vertpair last_pair;
        do {
          neighbors.push_back(loopf);

          int i;
          vertpair p;
          for (i = 0; i < loopf->connectivity.size(); i++) {
            p = loopf->connectivity[i];
            if (p != last_pair &&
                (p.first == v->id || p.second == v->id)) break;
          }

          last_pair = p;
          loopf = loopf->neighbors[i];
        } while (loopf != f);

        vector<Vector3f> normals;
        for (int i = 0; i < neighbors.size(); i++) {
          normals.push_back(neighbors[i]->normal);
        }
        vector<float> weights;

        if (mode == AVERAGE) {
          for (int i = 0; i < normals.size(); i++)
            weights.push_back(1);
        } else if (mode == AREA_WEIGHTS) {
          for (int i = 0; i < normals.size(); i++)
            weights.push_back(neighbors[i]->area());
        } else if (mode == ANGLE_WEIGHTS) {
          for (int i = 0; i < normals.size(); i++) {
            face *f = neighbors[i];
            int vidx = 0; while (vidx < 3 && f->verts[vidx] != v) vidx++;
            Vector3f e1 = f->verts[vidx]->loc - f->verts[(vidx+1)%3]->loc,
                     e2 = f->verts[vidx]->loc - f->verts[(vidx+2)%3]->loc;
            weights.push_back(e1.dot(e2));
          }
        }

        v->normal.setZero();
        float total_weight = 0;
        for (int i = 0; i < normals.size(); i++) {
          v->normal += normals[i] * weights[i];
          total_weight += weights[i];
        }
        v->normal.normalize();

        for (auto nit = neighbors.begin(); nit != neighbors.end(); nit++) {
          face *neighbor = *nit;
          bool found = false;
          for (int i = 0; !found && i < neighbor->verts.size(); i++) {
            found |= v == (neighbor->verts[i]);
            if (found) neighbor->verts[i]->normal = v->normal;
          }
          if (!found) cout << "vertex not shared" << endl;
        }
      }
    }
  }
}
