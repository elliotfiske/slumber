#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

GameState::GameState(GLFWwindow *window_) {
	camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0);
	for(int i = 0; i < 10; i++) {
	   Actor *newActor = new Actor(vec3(i, 0.0, 4.0), vec3(0.0, 0.0, 1.0), 1.0, 1.0);
	   newActor->posID = assets.pos_sphereID;
	   newActor->norID = assets.nor_sphereID;
	   newActor->indID = assets.ind_sphereID;
	   newActor->numVerts = assets.numVerts_sphere;
		actors.push_back(*newActor);
	}
	groundPlane = new Actor(vec3(0.0, 0.0, 0.0), vec3(0.0, -30.0, 0.0), 0.0, 0.0);
	//groundPlane->posID = assets.pos_groundID;
	//groundPlane->norID = assets.nor_groundID;
	//groundPlane->indID = assets.ind_groundID;
	//groundPlane->numVerts = assets.numVerts_ground;
groundPlane->posID = assets.pos_sphereID;
	groundPlane->norID = assets.nor_sphereID;
	groundPlane->indID = assets.ind_sphereID;
	groundPlane->numVerts = assets.numVerts_sphere;
	
	control = *new Control();
	control.updateControl();
	
	window = window_;
	prevTime = glfwGetTime();
}

void GameState::update(){
   double curTime = glfwGetTime();
   double elapsedTime = curTime - prevTime;
   prevTime = curTime;
   for(int i = 0; i < 10; i++){
      actors[i].step(elapsedTime);
   }
}

void GameState::setView() {
  glm::mat4 cam = glm::lookAt(camera->center, camera->center + camera->direction, glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(assets.h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(cam));
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
  glm::mat4 Projection = glm::perspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 200.f);
  glUniformMatrix4fv(assets.h_uProjMatrix, 1, GL_FALSE, glm::value_ptr(Projection));
}

void GameState::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(assets.ShadeProg);
   setView();
   setPerspectiveMat();
   glUniform3f(assets.h_uMatAmb, 0.02, 0.02, 0.1);
   glUniform3f(assets.h_uMatDif, 0.0, 0.08, 0.5);
   glUniform3f(assets.h_uMatSpec, 0.14, 0.14, 0.4);
   glUniform1f(assets.h_uMatShine, 120.0);
   for(int i = 0; i < 10; i++){
      actors[i].draw(assets);
   }
   groundPlane->draw(assets);
   glDisableVertexAttribArray(assets.h_aPosition);
   glDisableVertexAttribArray(assets.h_aNormal);
   
   glfwSwapBuffers(window);
   glfwPollEvents();
}
