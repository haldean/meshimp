#include <iostream>
#include <fstream>

#include "glinclude.h"
#include "mesh.h"
#include "objparse.h"
#include "drawmesh.h"
#include "trackball.h"
#include "simplify.h"

#define kRotationStep 10.0f
#define kTranslationStep .3f
#define BUFFER_LENGTH 64 

using namespace std;

// Lights & Materials

GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

// Parameters

drawopts drawOptions;
GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;
GLboolean isSmooth;

GLint viewport[4];
GLdouble modelview[16];
GLdouble projection[16];

char titleString[150];

mesh globalMesh;
void drawGlobalMesh() {
  drawMesh(globalMesh, drawOptions);
}

void initLights() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}

void setCamera() {
  glTranslatef(0, 0, camPosZ);
  glRotatef(camRotX, 1, 0, 0);
  glRotatef(camRotY, 0, 1, 0);
}

void setupRC()
{
  glClearColor(.2, .2, .2, 1.);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  initLights();

  // Place Camera
  camRotX = 350.0f;
  camRotY = 680.0f;
  camPosX = 0.0f;
  camPosY = 0.0f;
  camPosZ = -10.5f;

  isSmooth = true;
}

//
// This is the display callback:
//
void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();	{

    setCamera();
    glRotatef(85.0, 1.0, 1.0, 1.0);

    drawGlobalMesh();

    // Retrieve current matrice before they popped.
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

  } glPopMatrix();
  glFlush();
}



// Keyboard handling:
//
void specialKeys(unsigned char key, int x, int y) {
  if (key == 'w') {
    camRotX += kRotationStep;
  } else if (key == 's') {
    camRotX -= kRotationStep;
  } else if (key == 'a') {
    camRotY += kRotationStep;
  } else if (key == 'd') {
    camRotY -= kRotationStep;

  } else if (key == 'e') {
    camPosZ += kTranslationStep;
  } else if (key == 'q') {
    camPosZ -= kTranslationStep;

  } else if (key == 'm') {
    isSmooth = !isSmooth;
    if (isSmooth) {
      glShadeModel(GL_SMOOTH);
    } else {
      glShadeModel(GL_FLAT);
    }

  } else if (key == 'g') {
    drawOptions.drawEdges = !drawOptions.drawEdges;
  } else if (key == 'f') {
    drawOptions.drawFaces = !drawOptions.drawFaces;
  } else if (key == 'n') {
    drawOptions.drawNormals = !drawOptions.drawNormals;

  } else if (key == '1') {
    globalMesh.calculateNormals(NO_NORMALS);
  } else if (key == '2') {
    globalMesh.calculateNormals(AVERAGE);
  } else if (key == '3') {
    globalMesh.calculateNormals(AREA_WEIGHTS);
  } else if (key == '4') {
    globalMesh.calculateNormals(ANGLE_WEIGHTS);
 
  } else if (key == ',') {
    simplifyMesh(globalMesh, 0.5);
  } else if (key == '.') {
    simplifyMesh(globalMesh, 0.75);
  }

  // Refresh the Window
  glutPostRedisplay();
}

// If the veiwport is resized:
void changeSize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the clipping volume
  gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, .001f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " [obj file]" << endl;
    return 1;
  }
  ifstream objFile(argv[1]);
  loadObjFile(objFile, globalMesh);

  drawOptions = defaultDrawOptions();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("B-Spline Curves & Surfaces");
  setupRC();
  glutReshapeFunc(changeSize);
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(specialKeys);
  glutMainLoop();
  return 0;
}

