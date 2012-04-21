#ifndef __DRAWMESH_H__
#define __DRAWMESH__

#include "glinclude.h"
#include "mesh.h"

struct draw_options_struct {
  bool drawEdges;
  bool drawNormals;
  bool drawFaces;
  GLfloat edgeColor[4];
  GLfloat meshColor[4];
  GLfloat normalColor[4];
} typedef drawopts;

drawopts defaultDrawOptions();
void drawMesh(mesh&, drawopts);

#endif
