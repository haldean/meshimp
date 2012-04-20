#include "objparse.h"

#include <cstdio>
#include <iterator>
#include <vector>

#define NO_NORM 0

struct edge {
  unsigned int id;
  struct vertex* vertex;
  struct face* face;
  edge* next;
  edge* opposite;
};

struct vertex {
  unsigned int id;
  double x, y, z;
  struct edge* edge;

  void print() {
    cout << "V" << id << ": " << x << ", " << y << ", " << z;
  }
};

struct normal {
  unsigned int id;
  double x, y, z;
};

struct face {
  unsigned int id;
  struct edge* edge;
  struct normal* normal;
};

struct objface {
  unsigned int id;
  vector<unsigned int> vids;
  vector<unsigned int> nids;
};

vector<struct vertex *> verteces;
vector<struct normal *> normals;
vector<struct edge *> edges;
vector<struct objface> objfaces;
vector<struct face *> faces;

int parseVertexSpec(const string vspec) {
  unsigned int vid;
  sscanf(vspec.c_str(), "%d", &vid);
  return vid;
}

int parseNormalSpec(const string vspec) {
  unsigned int nid;
  unsigned long last_slash = vspec.find_last_of('/');
  if (last_slash == string::npos) return NO_NORM;

  sscanf(vspec.substr(last_slash).c_str(), "/%d", &nid);
  return nid;
}

void parseLine(string line) {
  if (line[0] == 'v' && line[1] == ' ') {
    struct vertex * v = new struct vertex();

    sscanf(line.c_str(), "v %lf %lf %lf", &v->x, &v->y, &v->z);
    v->id = verteces.size() + 1;

    verteces.push_back(v);
  } else if (line[0] == 'v' && line[1] == 'n') {
    struct normal * n = new struct normal();

    sscanf(line.c_str(), "vn %lf %lf %lf", &n->x, &n->y, &n->z);
    n->id = normals.size() + 1;

    normals.push_back(n);
  } else if (line[0] == 'f' && line[1] == ' ') {
    istringstream tokenizer(line);
    vector<string> tokens;
    copy(istream_iterator<string>(tokenizer),
        istream_iterator<string>(),
        back_inserter<vector<string> >(tokens));

    struct objface f;
    f.id = objfaces.size() + 1;
    for (unsigned int i=1; i<tokens.size(); i++) {
      string token = tokens[i];
      f.vids.push_back(parseVertexSpec(token));
      f.nids.push_back(parseNormalSpec(token));
    }

    objfaces.push_back(f);
  }
}

/*
void mergeHalfEdges() {
  map<pair<unsigned int, unsigned int>, struct edge *> halfedges;
  for (vector<struct edge *>::const_iterator edge_iter = edges.begin();
       edge_iter != edges.end(); edge_iter++) {
    struct edge * edge = *edge_iter;

    int vid1 = edge->vertex->id;
    int vid2 = edge->next->next->vertex->id;
    pair<unsigned int, unsigned int> vids = 
      pair<unsigned int, unsigned int>(
        vid1 < vid2 ? vid1 : vid2, vid1 < vid2 ? vid2 : vid1);

    map<pair<unsigned int, unsigned int>, struct edge *>::iterator
      mapval = halfedges.find(vids);

    if (mapval == halfedges.end()) {
      halfedges.insert(
          pair<pair<unsigned int, unsigned int>, struct edge *>(vids, edge));
    } else {
      struct edge * edge_ = mapval->second;
      edge->opposite = edge_;
      edge_->opposite = edge;
    }
  }
}

void addTriangle(vector<unsigned int> vids, vector<unsigned int> nids) {
  struct face * f = new struct face();
  f->id = faces.size() + 1;

  struct edge *e0 = new struct edge(),
              *e1 = new struct edge(),
              *e2 = new struct edge();

  struct vertex *v0 = verteces[vids[0] - 1],
                *v1 = verteces[vids[1] - 1],
                *v2 = verteces[vids[2] - 1];

  e0->vertex = v0;
  v2->edge = e0;
  e0->face = f;
  e0->next = e1;
  e0->opposite = NULL;
  e0->id = edges.size() + 1;
  edges.push_back(e0);

  e1->vertex = v1;
  v0->edge = e1;
  e1->face = f;
  e1->next = e2;
  e1->opposite = NULL;
  e1->id = edges.size() + 1;
  edges.push_back(e1);

  e2->vertex = v2;
  v1->edge = e2;
  e2->face = f;
  e2->next = e0;
  e2->opposite = NULL;
  e2->id = edges.size() + 1;
  edges.push_back(e2);

  f->edge = e0;

  bool normals_defined = true;
  for (vector<unsigned int>::const_iterator normid = nids.begin();
      normid != nids.end(); normid++) {
    normals_defined &= *normid != NO_NORM;
  }

  if (normals_defined) {
    struct normal * n0 = normals[nids[0] - 1];
    struct normal * n1 = normals[nids[1] - 1];
    struct normal * n2 = normals[nids[2] - 1];
    f->normal = normalize(
        n0->x + n1->x + n2->x,
        n0->y + n1->y + n2->y,
        n0->z + n1->z + n2->z);
  } else {
    struct vertex * v0 = verteces[vids[0] - 1];
    struct vertex * v1 = verteces[vids[1] - 1];
    struct vertex * v2 = verteces[vids[2] - 1];
    f->normal = normForFace(v0, v1, v2);
  }

  faces.push_back(f);
}
*/

Vector3f normalToVector(uint nid) {
  struct normal* norm = normals[nid - 1];
  assert(norm->id == nid);
  return Vector3f(norm->x, norm->y, norm->z);
}

Vector3f vertexToVector(uint vid) {
  struct vertex* vert = verteces[vid - 1];
  assert(vert->id == vid);
  return Vector3f(vert->x, vert->y, vert->z);
}

int objfaceToTriangles(
    Scene& scene, Material* material, Vector3f& min, Vector3f& max) {
  int tris = 0;
  min << INFINITY, INFINITY, INFINITY;
  max << -INFINITY, -INFINITY, -INFINITY;

  for (vector<struct objface>::iterator iter = objfaces.begin();
       iter != objfaces.end(); iter++) {
    const struct objface objf = *iter;

    int verts = objf.vids.size();
    if (verts < 3) continue;

    for (int i=0; i<verts; i++) {
      struct vertex* vert = verteces[objf.vids[i] - 1];
      if (vert->x < min[0]) min[0] = vert->x;
      if (vert->y < min[1]) min[1] = vert->y;
      if (vert->z < min[2]) min[2] = vert->z;
      if (vert->x > max[0]) max[0] = vert->x;
      if (vert->y > max[1]) max[1] = vert->y;
      if (vert->z > max[2]) max[2] = vert->z;
    }

    for (int i=1; i<verts-1; i++) {
      bool normals_defined = 
        objf.nids[0] != NO_NORM && objf.nids[i] != NO_NORM
        && objf.nids[i+1] != NO_NORM;

      Triangle* t;
      if (normals_defined) {
        t = new Triangle(
              vertexToVector(objf.vids[0]),
              vertexToVector(objf.vids[i]),
              vertexToVector(objf.vids[i+1]),
              normalToVector(objf.nids[0]),
              normalToVector(objf.nids[i]),
              normalToVector(objf.nids[i+1]));
      } else {
        t = new Triangle(
              vertexToVector(objf.vids[0]),
              vertexToVector(objf.vids[i]),
              vertexToVector(objf.vids[i+1]));
      }
      t->material = material;
      scene.geom.addGeometry(t);

      tris++;
    }
  }

  return tris;
}

void loadObjFile(istream& file, Scene& scene, Material* material) {
  string line;
  while (file.good()) {
    getline(file, line);
    parseLine(line);
  }
  Vector3f min, max;
  int tris = objfaceToTriangles(scene, material, min, max);

  cout << "Loaded mesh: " << endl
    << "  " << verteces.size() << " verteces." << endl
    << "  " << normals.size() << " normals." << endl
    << "  " << objfaces.size() << " faces in OBJ file." << endl
    << "  " << tris << " triangular faces." << endl
    << "  bounds: " << endl
    << "    " << min[0] << " " << min[1] << " " << min[2] << endl
    << "    " << max[0] << " " << max[1] << " " << max[2] << endl;
}

