#include "glm/glm.hpp"

#ifndef PLANE_H
#define PLANE_H

class Plane {
public:
   float a, b, c, d;

   Plane(void);
   ~Plane();

   void setCoefficients(float a, float b, float c, float d);
   float distance(glm::vec3 p);
   void makeNormal();
   
};

#endif