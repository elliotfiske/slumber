#include "Plane.hpp"
#define INSIDE 1
#define OUTSIDE -1
#define INTERSECT 0

class ViewFrustum{
public:
   Plane planes[6];
    Plane leftPlane;
    Plane rightPlane;
    Plane nearPlane;
    Plane farPlane;
    Plane topPlane;
    Plane bottomPlane;

   ViewFrustum();
   ~ViewFrustum();

   void extractPlanes(glm::mat4 comboMatrix, bool normalize);
   int sphereIsInside(glm::vec3 point, int radius);
};
