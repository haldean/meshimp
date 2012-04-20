#ifndef __OBJPARSE_H__
#define __OBJPARSE_H__

#include <istream>

#include "mesh.h"

void loadObjFile(istream& input, mesh& mesh);

#endif
