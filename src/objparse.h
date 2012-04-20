#ifndef __CRENDER_OBJPARSE_H__
#define __CRENDER_OBJPARSE_H__

#include <istream>

#include "scene.h"
#include "geometry.h"

void loadObjFile(istream& input, Scene& scene, Material* material);

#endif
