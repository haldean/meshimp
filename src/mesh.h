#ifndef __MESH_H__
#define __MESH_H__

#include <Eigen/Dense>
#include <vector>
#include <iostream>

using namespace Eigen;
using namespace std;

enum normal_mode {
  NO_NORMALS,
  AVERAGE,
  AREA_WEIGHTS,
  ANGLE_WEIGHTS
};

typedef pair<int, int> vertpair;
vertpair makeVertpair(int v1, int v2);

class vertex {
  public:
    vertex();
    vertex(const vertex &other);
    bool operator==(const vertex &other);

    int id;
    Vector3f loc;
    Vector3f normal;
};

class face {
  public:
    face();
    face(const face &other);
    float area() const;

    int id;
    Vector3f normal;
    vector<vertex*> verts;
    vector<face*> neighbors;
    vector<vertpair> connectivity;
};

class mesh {
  public:
    mesh();
    void calculateNormals(normal_mode mode);

    vector<face*> faces;
    bool manifold;
    int max_vertex_id;
};

#endif
