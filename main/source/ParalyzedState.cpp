//
//  ParalyzedState.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#include "ParalyzedState.h"
#include "network.h"
#include "control.hpp"

#ifdef THREADS
    #include <thread>
#endif

ParalyzedState::ParalyzedState(GLFWwindow *window): GameState(window, false) {
    playerHealth = 100;
    playerSensitivity = false;
    camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    CurrAssets->currFBOShader = CurrAssets->currShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
#ifdef THREADS
    thread *t1;
    
    t1 = new thread(doClientNetworking);
#endif
}

void ParalyzedState::checkCollisions() {
    mat4 comboMatrix;
    
    comboMatrix = perspectiveMat * viewMat * collectible->modelMat;
    vf->extractPlanes(comboMatrix);
    
    if (vf->gotLight(collectible->center, 5.0)) {
        collectible->collected();
    }
}

void ParalyzedState::update() {
    GameState::update();
    
    Position ghostPos = getGhostPosition();
    enemy->center.x = ghostPos.x;
    enemy->center.y = ghostPos.y;
    enemy->center.z = ghostPos.z;
    
    tellGhostWhereImLooking();
}


/**
 * Send a packet to the ghost player with the current
 *  location of where I'm looking
 */
void ParalyzedState::tellGhostWhereImLooking() {
#ifdef THREADS
	float pitch = getPitch();
	float yaw = getYaw();

    sendPlayerLook(pitch, yaw);
#endif
}

/**
 * Draw the scene from the user's perspective
 */
void ParalyzedState::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glCullFace(GL_BACK);
    
    updateViewMat();
    
    CurrAssets->lightingShader->startUsingShader();
    CurrAssets->lightingShader->setViewMatrix(viewMat);
    CurrAssets->lightingShader->setProjectionMatrix(perspectiveMat);
	CurrAssets->lightingShader->setHighlightVP(highlightVPMat);
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID, 1);
    
    //    viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    tv->draw(light);
    lamp->draw(light);
    door->draw(light);
    
    shadowfbo->unbindTexture();

    CurrAssets->collectibleShader->startUsingShader();
    CurrAssets->collectibleShader->setViewMatrix(viewMat);
    CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);
    
    collectible->draw(light);
    
    // check OpenGL error TODO: remove
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << endl;
    }
}
void ParalyzedState::increaseHealth(int healthValue){
   if(playerHealth < 100){
      playerHealth += healthValue;
   }
   else {
      playerHealth = 100;
   }
}
void ParalyzedState::lowerHealth(int severity){
   if(playerSensitivity = false){
      playerHealth -= severity;
   }
   else {
      playerHealth -= (severity + (severity/2));
   }
}
void ParalyzedState::increaseSensitive(){
   playerSensitivity = true;
}
void ParalyzedState::lowerSensitive(){
   playerSensitivity = false;
}

int ParalyzedState::getHealth(){
   return playerHealth;
}
    
bool ParalyzedState::getSensitivity(){
   return playerSensitivity;
}
