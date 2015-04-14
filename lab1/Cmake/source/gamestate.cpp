#include "gamestate.hpp"
#include "GLSL.h"

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

void GameState::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   glUseProgram(assets.ShadeProg);
   
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
   9999,
   GL_UNSIGNED_INT,
   (void*)0
   );
   
   glDisableVertexAttribArray(assets.h_aPosition);
   glDisableVertexAttribArray(assets.h_aNormal);
   
   glfwSwapBuffers(window);
   glfwPollEvents();
}
