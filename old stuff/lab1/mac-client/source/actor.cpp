#include "actor.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
Actor::Actor(vec3 center_, vec3 direction_, float velocityScale, float radius) {
   center = center_;
   direction = direction_;
   velocityScalar = velocityScale;
   boundSphereRad = radius;
}

void Actor::step(double dt) {
   vec3 curChange;
   curChange = (velocityScalar * (float) dt) * direction;
   center += curChange;

   collisionCooldown--;
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

void Actor::setModel(Assets assets) {
   glm::mat4 Trans = glm::translate(glm::mat4(1.0f), center);
   glm::mat4 RotX   = glm::rotate(glm::mat4(1.0f), direction.x, vec3(1, 0, 0));
   glm::mat4 RotY   = glm::rotate(glm::mat4(1.0f), direction.y, vec3(0, 1, 0));
   glm::mat4 RotZ   = glm::rotate(glm::mat4(1.0f), direction.z, vec3(0, 0, 1));
   
   glm::mat4 com = Trans * RotX * RotY * RotZ;
   glUniformMatrix4fv(assets.h_uModelMatrix, 1, GL_FALSE, glm::value_ptr(com));
}

void Actor::setMaterial(Assets assets) {
   glUniform3f(assets.h_uMatAmb,   ambientColor.x,  ambientColor.y,  ambientColor.z);
   glUniform3f(assets.h_uMatDif,   diffuseColor.x,  diffuseColor.y,  diffuseColor.z);
   glUniform3f(assets.h_uMatSpec,  specularColor.x, specularColor.y, specularColor.z);
   glUniform1f(assets.h_uMatShine, shininess);
}

void Actor::draw(Assets assets) {
   setModel(assets);
   setMaterial(assets);
   GLSL::enableVertexAttribArray(assets.h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, posID);
   glVertexAttribPointer(assets.h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   GLSL::enableVertexAttribArray(assets.h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, norID);
   glVertexAttribPointer(assets.h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indID);

   glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, (void*) 0);
}