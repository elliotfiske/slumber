#include "../libraries/actor.hpp"
#include "../libraries/GLSL.h"
#include "../libraries/assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
Actor::Actor(vec3 center_, vec3 direction_, float velocityScale, float radius) {
   center = center_;
   direction = direction_;
   velocityScalar = velocityScale;
   boundSphereRad = radius;
    
   scale = vec3(1.0, 1.0, 1.0);

   timeToDeath = -1;
}

void Actor::step(double dt) {
   vec3 curChange;
   curChange = (velocityScalar * (float) dt) * direction;
   center += curChange;

   collisionCooldown--;
   timeToDeath--;
}

bool Actor::detectIntersect(Actor target, bool overrideCooldown) {
   if (collisionCooldown > 0 && !overrideCooldown) {
      return false;
   }

   float dist = glm::distance(center, target.center);

   float minDist = this->boundSphereRad + target.boundSphereRad;
   bool result = dist <= minDist;

   if (result) {
      collisionCooldown = 18;
   }

   return result;
}

void Actor::die() {
   direction = vec3(0.0, 1.0, 0.0);
   velocityScalar = 3.0;

   timeToDeath = 60;
}

void Actor::setModel(Assets assets) {
   glm::mat4 Scale = glm::scale(glm::mat4(1.0f), scale);
   glm::mat4 Trans = glm::translate(glm::mat4(1.0f), center);
   glm::mat4 com = Trans * Scale;
   glUniformMatrix4fv(assets.h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(com));
}

void Actor::draw(Assets assets) {

   setModel(assets);
   GLSL::enableVertexAttribArray(assets.h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posID);
   glVertexAttribPointer(assets.h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(assets.h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norID);
   glVertexAttribPointer(assets.h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indID);

   glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, (void*) 0);
}
