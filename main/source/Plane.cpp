#include "Plane.hpp"
#include <stdio.h>

Plane::Plane() {}

Plane::~Plane() {}

void Plane::setCoefficients(float a, float b, float c, float d) {
   this.a = a;
   this.b = b;
   this.c = c;
   this.d = d;
}
   

float Plane::distance(vec3 p) {
   return this.a*p.x + this.b*p.y + this.c*p.z + this.d;
}

void normalize(){
   float mag;
   mag = sqrt(this.a * this.a + this.b * this.b + this.c * this.c);
   this.a = this.a / mag;
   this.b = this.b / mag;
   this.c = this.c / mag;
   this.d = this.d / mag;
}

Halfspace classifyPoint(vec3 pt){
   float d;
   d = this.a*pt.x + this.b*pt.y + this.c*pt.z + this.d;
   if (d < 0) return NEGATIVE;
   if (d > 0) return POSITIVE;
   return ON_PLANE;
}