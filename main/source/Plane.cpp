#include "Plane.hpp"
#include "glm/glm.hpp"
#include <math.h>
#include <stdio.h>

Plane::Plane() {}

Plane::~Plane() {}

void Plane::setCoefficients(float a_, float b_, float c_, float d_) {
   a = a_;
   b = b_;
   c = c_;
   d = d_;
}

float Plane::distance(glm::vec3 p) {
   return a*p.x + b*p.y + c*p.z + d;
}

void Plane::makeNormal(){
   float mag;
   mag = sqrt((a * a) + (b * b) + (c * c));
   a = a / mag;
   b = b / mag;
   c = c / mag;
   d = d / mag;
}

