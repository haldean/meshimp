#include "objparse.h"

#include <cstdio>
#include <iterator>
#include <vector>
#include <map>
#include <iostream>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

struct objface {
  unsigned int id;
  vector<unsigned int> vids;
};

typedef pair<face*, face*> tripair;
vertpair makeVertpair(int v1, int v2) {
  if (v1 < v2) {
    return vertpair(v1, v2);
  } else {
    return vertpair(v2, v1);
  }
}
map<vertpair, tripair> neighborLookup;

vector<vertex*> verteces;
vector<struct objface> faces;

void parseLine(string line) {
  if (line[0] == 'v' && line[1] == ' ') {
    vertex *v = new vertex();

    float x, y, z;
    sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
    v->loc = Vector3f(x, y, z);

    v->id = verteces.size() + 1;
    v->normal = Vector3f::Zero();

    verteces.push_back(v);

  } else if (line[0] == 'f' && line[1] == ' ') {
    istringstream tokenizer(line);
    vector<string> tokens;
    copy(istream_iterator<string>(tokenizer),
        istream_iterator<string>(),
        back_inserter<vector<string> >(tokens));

    struct objface f;
    f.id = faces.size() + 1;
    for (unsigned int i=1; i<tokens.size(); i++) {
      string token = tokens[i];
      unsigned int vid;
      sscanf(token.c_str(), "%d", &vid);
      f.vids.push_back(vid);
    }

    faces.push_back(f);
  }
}

void insertFaceForVertPair(int v1, int v2, face *f) {
  vertpair vp = makeVertpair(v1, v2);
  tripair tp = neighborLookup[vp];
  if (tp.first == NULL) {
    tp.first = f;
  } else {
    tp.second = f;
  }
  neighborLookup[vp] = tp;
}

void insertFaceIntoNeighborMap(struct objface objf, face *f) {
  insertFaceForVertPair(objf.vids[0], objf.vids[1], f);
  insertFaceForVertPair(objf.vids[0], objf.vids[2], f);
  insertFaceForVertPair(objf.vids[1], objf.vids[2], f);
}

bool assignNeighbors() {
  bool manifold = true;
  for (auto it = neighborLookup.begin(); it != neighborLookup.end(); it++) {
    vertpair vp = (*it).first;
    tripair tp = (*it).second;
    if (tp.first != NULL && tp.second != NULL) {
      tp.first->neighbors.push_back(tp.second);
      tp.second->neighbors.push_back(tp.first);

      tp.first->connectivity.push_back(vp);
      tp.second->connectivity.push_back(vp);
    } else {
      manifold = false;
    }
  }
  return manifold;
}

void calculateFaceNormal(face *f) {
  Vector3f e1 = f->verts[0]->loc - f->verts[1]->loc,
           e2 = f->verts[0]->loc - f->verts[2]->loc;
  f->normal = e1.cross(e2);
  f->normal.normalize();
}

void objfacesToMesh(mesh& mesh) {
  for (int i = 0; i < faces.size(); i++) {
    struct objface objf = faces[i];
    face *f = new face();
    f->id = objf.id;
    for (int j = 0; j < 3; j++) {
      f->verts[j] = verteces[objf.vids[j] - 1];
    }
    calculateFaceNormal(f);
    mesh.faces.push_back(f);
    insertFaceIntoNeighborMap(objf, f);
  }

  mesh.manifold = assignNeighbors();
  mesh.max_vertex_id = verteces.size();
}

void loadObjFile(istream& file, mesh& mesh) {
  string line;
  while (file.good()) {
    getline(file, line);
    parseLine(line);
  }

  objfacesToMesh(mesh);

  cout << "Loaded mesh: " << endl
    << "  " << verteces.size() << " verteces." << endl
    << "  " << faces.size() << " faces in OBJ file." << endl
    << "  new global mesh size: " << mesh.faces.size() << endl
    << "  global mesh has " << (mesh.manifold ? "no " : "") << "boundaries" << endl;
}

