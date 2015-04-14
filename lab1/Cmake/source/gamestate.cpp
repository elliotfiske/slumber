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
   glfwSwapBuffers(window);
}
