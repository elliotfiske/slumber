#include <glm/glm.hpp>
#include "Plane.hpp"
#define INSIDE 1
#define OUTSIDE -1;
#define INTERSECT 0;

class ViewFrustum{

   ViewFrustum::ViewFrustum(void);
   ViewFrustum::~ViewFrustum();

   Plane[6] planes;
   void extractPlanes(mat4 comboMatrix, bool normalize);
   bool sphereIsInside(vec3 point, int radius);
};