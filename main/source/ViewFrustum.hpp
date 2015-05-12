#include "Plane.hpp"
#define INSIDE 1
#define OUTSIDE -1
#define INTERSECT 0

class ViewFrustum{
public:
   Plane planes[6];

   ViewFrustum();
   ~ViewFrustum();

   void extractPlanes(glm::mat4 comboMatrix);
   int sphereIsInside(glm::vec3 point, float radius);
};
