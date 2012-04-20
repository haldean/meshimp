#include <iostream>
#include <fstream>

#include "objparse.h"
#include "mesh.h"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Must provide OBJ file to test." << endl;
    return 1;
  }

  mesh m;
  ifstream objfile(argv[1]);
  loadObjFile(objfile, m);
  return 0;
}
