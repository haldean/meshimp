#include "objparse.h"

#include <cstdio>
#include <iterator>
#include <vector>
#include <map>
#include <iostream>

#include <Eigen/Core>

using namespace std;
using namespace Eigen;

struct objvertex {
  unsigned int id;
  double x, y, z;
  struct edge* edge;

  void print() {
    cout << "V" << id << ": " << x << ", " << y << ", " << z;
  }
};

struct objedge {
  unsigned int id;
  struct objvertex* vertex;
  struct objface* face;
  struct objedge* next;
  struct objedge* opposite;
};

struct objface {
  unsigned int id;
  vector<unsigned int> vids;
};

typedef pair<struct objface*, struct objface*> tripair;
typedef pair<struct objvertex*, struct objvertex*> vertpair;
vertpair getVertPair(struct objvertex* v1, struct objvertex* v2) {
  if (v1->id < v2->id) {
    return vertpair(v1, v2);
  } else {
    return vertpair(v2, v1);
  }
}
map<vertpair, tripair> neighborLookup;

vector<struct objvertex *> verteces;
vector<struct objedge *> edges;
vector<struct objface> faces;

int parseVertexSpec(const string vspec) {
  unsigned int vid;
  sscanf(vspec.c_str(), "%d", &vid);
  return vid;
}

void parseLine(string line) {
  if (line[0] == 'v' && line[1] == ' ') {
    struct objvertex *v = new struct objvertex();

    sscanf(line.c_str(), "v %lf %lf %lf", &v->x, &v->y, &v->z);
    v->id = verteces.size() + 1;

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

Vector3f vertexToVector(unsigned int vid) {
  struct objvertex* vert = verteces[vid - 1];
  assert(vert->id == vid);
  return Vector3f(vert->x, vert->y, vert->z);
}

vertex* objvertToVertex(objvertex *objv) {
  vertex *v = new vertex();
  v->loc = Vector3f(objv->x, objv->y, objv->z);
  v->normal = Vector3f::Zero();
  return v;
}

void objfacesToMesh(mesh& mesh) {
  for (int i = 0; i < faces.size(); i++) {
    struct objface objf = faces[i];
    face *f = new face();
    for (int j = 0; j < 3; j++) {
      f->verts[j] = objvertToVertex(verteces[objf.vids[j] - 1]);
      f->neighbors.push_back(NULL);
    }
    mesh.faces.push_back(f);
  }
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
    << "  new global mesh size: " << mesh.faces.size() << endl;
}

