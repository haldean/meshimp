#include <Eigen/Core>
#include <iostream>
#include <cstring>

#include "drawmesh.h"
#include "glinclude.h"

void vector3ToGL(Vector3f v) {
  glVertex3f(v[0], v[1], v[2]);
}

void drawVertex(vertex &v, face *face, drawopts opts) {
  if (!v.normal.isZero()) {
    glNormal3f(v.normal[0], v.normal[1], v.normal[2]);
  } else {
    glNormal3f(face->normal[0], face->normal[1], face->normal[2]);
  }
  vector3ToGL(v.loc);
}

void drawFace(face* face, drawopts opts) {
      //(GLfloat[]){0.5, 0.5, 0.5, 1.});
  drawVertex(*(face->verts[0]), face, opts);
  drawVertex(*(face->verts[1]), face, opts);
  drawVertex(*(face->verts[2]), face, opts);
}

void drawEdges(face* face, drawopts opts) {
  glLineWidth(3.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, opts.edgeColor);
  glBegin(GL_LINE_STRIP); {
    vector3ToGL(face->verts[0]->loc);
    vector3ToGL(face->verts[1]->loc);
    vector3ToGL(face->verts[2]->loc);
    vector3ToGL(face->verts[0]->loc);
  } glEnd();
}

void drawNormals(face* face, drawopts opts) {
  glLineWidth(1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, opts.normalColor);

  for (int i = 0; i < 3; i++) {
    vertex *v = face->verts[i];
    glBegin(GL_LINE_STRIP); {
      vector3ToGL(v->loc);
      vector3ToGL(v->loc + v->normal);
    } glEnd();
  }
}

void drawMesh(mesh &mesh, drawopts opts) {
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, opts.meshColor);
  if (opts.drawFaces) {
    glBegin(GL_TRIANGLES); {
      for (auto it = mesh.faces.begin(); it != mesh.faces.end(); it++) {
        face* face = (*it);
        drawFace(face, opts);
      }
    } glEnd();
  }

  if (opts.drawEdges || opts.drawNormals) {
    for (auto it = mesh.faces.begin(); it != mesh.faces.end(); it++) {
      face* face = (*it);
      if (opts.drawEdges) drawEdges(face, opts);
      if (opts.drawNormals) drawNormals(face, opts);
    }
  }
}

drawopts defaultDrawOptions() {
  drawopts opts;
  opts.drawEdges = false;
  opts.drawNormals = false;
  opts.drawFaces = true;

  opts.normalColor[0] = 1.;
  opts.normalColor[1] = .5;
  opts.normalColor[2] = .5;
  opts.normalColor[3] = 1.;

  for (int i=0; i<3; i++) opts.edgeColor[i] = 0.;
  opts.edgeColor[3] = 1.;

  opts.meshColor[0] = 1.;
  opts.meshColor[1] = .5;
  opts.meshColor[2] = 0.;
  opts.meshColor[3] = 1.;

  return opts;
}
