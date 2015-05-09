#include <glm/glm.hpp>

class Plane{
   float a, b, c, d;

   Plane::Plane(void);
   Plane::~Plane();

   void setCoefficients(float a, float b, float c, float d);
   float distance(vec3 p);
   void normalize();
   
   enum Halfspace{
      NEGATIVE = -1,
      ON_PLANE = 0,
      POSITIVE = 1,
   };
   
   Halfspace classifyPoint(const Plane & plane, const Point & pt);
}