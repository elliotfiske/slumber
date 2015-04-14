#include "actor.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

void Actor::setModel() {
   glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
   glm::mat4 com = Trans;
}

void Actor::draw(Assets assets){


   GLSL::enableVertexAttribArray(assets.h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, assets.pos_sphereID);
   glVertexAttribPointer(assets.h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   
   GLSL::enableVertexAttribArray(assets.h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, assets.nor_sphereID);
   glVertexAttribPointer(assets.h_aNormal,
                           3,
                           GL_FLOAT,
                           GL_FALSE,
                           0,
                           (void*) 0
                           );
   
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, assets.ind_sphereID);
   
   glDrawElements(
   GL_TRIANGLES,
   assets.numVerts_sphere,
   GL_UNSIGNED_INT,
   (void*)0
   );
}
