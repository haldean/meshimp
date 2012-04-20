#ifndef __MESH_H__
#define __MESH_H__

#include <Eigen/Core>
#include <vector>
#include <iostream>

using namespace Eigen;
using namespace std;

typedef float scalar;

class vertex {
  public:
    vertex();
    vertex(const vertex &other);
    Vector3f loc;
    Vector3f normal;
};

class face {
  public:
    face();
    face(const face &other);
    vector<vertex*> verts;
    vector<face*> neighbors;
};

class mesh {
  public:
    mesh();
    vector<face*> faces;
};

#endif
