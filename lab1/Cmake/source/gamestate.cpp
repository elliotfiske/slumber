#include "gamestate.hpp"

GameState::GameState(){
	camera.push_back(*new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0));
	for(int i = 0; i < 10; i++) {
		actors.push_back(*new Actor(vec3(i, 0.0, 0.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0));
	}
	control = *new Control();
	control.updateControl();
}

void GameState::update(){
   
}

void GameState::draw(){

}
