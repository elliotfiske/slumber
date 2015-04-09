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
