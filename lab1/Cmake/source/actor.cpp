#include "actor.hpp"

Actor::Actor(vec3 center_, vec3 direction_, float velocityScale, float radius) {
   center = center_;
   direction = direction_;
   velocityScalar = velocityScale;
   boundSphereRad = radius;
}

void Actor::step(float dt){
   vec3 curChange;
   curChange = (velocityScalar * dt) * direction;
   center += curChange;
}

bool Actor::detectIntersect(Actor target) {
   vec3 relPosition = this->center - target.center;
   float dist = relPosition.x * relPosition.x + relPosition.y * relPosition.y + relPosition.z * relPosition.z;

   float minDist = this->boundSphereRad + target.boundSphereRad;
   return dist <= minDist * minDist;
}
