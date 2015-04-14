#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

GameState::GameState(GLFWwindow *window_) {
	camera.push_back(*new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0));
	for(int i = 0; i < 10; i++) {
		actors.push_back(*new Actor(vec3(i, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0));
	}
	control = *new Control();
	control.updateControl();
	
	window = window_;
}

void GameState::update(){
   
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
  glm::mat4 Projection = glm::perspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 200.f);
  safe_glUniformMatrix4fv(assets.h_uProjMatrix, glm::value_ptr(Projection));
}

void GameState::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glUseProgram(assets.ShadeProg);
   
   for(int i = 0; i < 10; i++){
      actors[i].draw(assets);
   }
   
   glDisableVertexAttribArray(assets.h_aPosition);
   glDisableVertexAttribArray(assets.h_aNormal);
   
   glfwSwapBuffers(window);
   glfwPollEvents();
}
