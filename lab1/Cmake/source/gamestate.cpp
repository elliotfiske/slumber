#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"

GameState::GameState(GLFWwindow *window_) {
   camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0);

   groundPlane = new Actor(vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, 0.0), 0.0, 0.0);
   groundPlane->posID = assets.pos_groundID;
   groundPlane->norID = assets.nor_groundID;
   groundPlane->indID = assets.ind_groundID;
   groundPlane->numVerts = assets.numVerts_ground;

   window = window_;

   setupCallbacks(window);

   prevTime = glfwGetTime();
   timeToNextSphere = 0;
}

void GameState::spawnSphere() {
   float randX = glm::linearRand(-XMAX, XMAX);
   float randZ = glm::linearRand(-ZMAX, ZMAX);

   float randAngle = glm::linearRand(0.0, 2 * 3.14159);
   float randXVel = cos(randAngle);
   float randZVel = sin(randAngle);

   Actor *newActor = new Actor(vec3(randX, 0.2, randZ), vec3(randXVel, 0.0,
         randZVel), 5.0, 1.0);
   newActor->posID = assets.pos_sphereID;
   newActor->norID = assets.nor_sphereID;
   newActor->indID = assets.ind_sphereID;
   newActor->numVerts = assets.numVerts_sphere;
   actors.push_back(*newActor);
}

void GameState::checkCollisions() {
   for (int i = 0; i < actors.size() - 1; i++) {
      for (int j = i + 1; j < actors.size(); j++) {
         if (actors[i].detectIntersect(actors[j], false)) {
            actors[i].direction.x = -actors[i].direction.x;
            actors[i].direction.z = -actors[i].direction.z;
            actors[i].velocityScalar++;

            actors[j].direction.x = -actors[j].direction.x;
            actors[j].direction.z = -actors[j].direction.z;
            actors[j].velocityScalar++;
         }
      }
   }

   for (int i = actors.size() - 1; i >= 0; i--) {

      printf("Hi\n");
      if (camera->detectIntersect(actors[i], true)) {
         actors[i].die();
      }

      if (actors[i].center.x < -XMAX) {
         actors[i].center.x = -XMAX;
         actors[i].direction.x = -actors[i].direction.x;
      }

      if (actors[i].center.x > XMAX) {
         actors[i].center.x = XMAX;
         actors[i].direction.x = -actors[i].direction.x;
      }

      if (actors[i].center.z < -ZMAX) {
         actors[i].center.z = -ZMAX;
         actors[i].direction.z = -actors[i].direction.z;
      }

      if (actors[i].center.z > ZMAX) {
         actors[i].center.z = ZMAX;
         actors[i].direction.z = -actors[i].direction.z;
      }

      if (actors[i].timeToDeath == 0) {
         actors.erase(actors.begin() + i);
      }
   }
}

void GameState::update() {

   updateControl(window);
   updateCamera(camera);

   double curTime = glfwGetTime();
   double elapsedTime = curTime - prevTime;
   prevTime = curTime;
   for (int i = 0; i < actors.size(); i++) {
      actors[i].step(elapsedTime);
   }

   if (curTime > timeToNextSphere) {
      timeToNextSphere = curTime + 0.5;

      spawnSphere();
   }

   checkCollisions();

   camera->center += camera->direction * (float) elapsedTime
         * getForwardVelocity();
   glm::vec3 right = glm::cross(camera->direction, glm::vec3(0.0, 1.0, 0.0));
   camera->center += right * (float) elapsedTime * getStrafeVelocity();
}

void GameState::setView() {
   glm::mat4 cam = glm::lookAt(camera->center, camera->center
         + camera->direction, glm::vec3(0.0, 1.0, 0.0));
   glUniformMatrix4fv(assets.h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(cam));
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
   glm::mat4 Projection = glm::perspective(45.0f, (float) WINDOW_WIDTH
         / WINDOW_HEIGHT, 0.1f, 200.f);
   glUniformMatrix4fv(assets.h_uProjMatrix, 1, GL_FALSE, glm::value_ptr(
         Projection));
}

void GameState::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(assets.ShadeProg);
   setView();
   setPerspectiveMat();
   for (int i = 0; i < actors.size(); i++) {
      if (actors[i].timeToDeath > 0) {
         glUniform3f(assets.h_uMatAmb, 0.1, 0.0, 0.0);
         glUniform3f(assets.h_uMatDif, 0.1, 0.0, 0.0);
      } else {
         glUniform3f(assets.h_uMatAmb, .988, .776, .255);
         glUniform3f(assets.h_uMatDif, .988, .776, .255);
         glUniform3f(assets.h_uMatSpec, 0.14, 0.14, 0.4);
         glUniform1f(assets.h_uMatShine, 120.0);
      }
      actors[i].draw(assets);
   }

   glUniform3f(assets.h_uMatAmb, .584, .102, 0);
   glUniform3f(assets.h_uMatDif, .012, .271, .369);
   glUniform3f(assets.h_uMatSpec, 0.02, 0.02, 0.04);
   glUniform1f(assets.h_uMatShine, 120.0);
   groundPlane->draw(assets);
   glDisableVertexAttribArray(assets.h_aPosition);
   glDisableVertexAttribArray(assets.h_aNormal);

   glfwSwapBuffers(window);
   glfwPollEvents();
}
