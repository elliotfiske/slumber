//
//  titlestate.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/15/15.
//
//

#include "titlestate.h"
#include "control.hpp"
#include "GhostState.h"
#include "ParalyzedState.h"

TitleState::TitleState(GLFWwindow *window): GameState(window, false) {
    camera = new Camera(vec3(-7.5, 5.0, -69.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    title = CurrAssets->billboardDictionary["title.png"];
    play = CurrAssets->billboardDictionary["play.png"];
    playGhost = CurrAssets->billboardDictionary["play_ghost.png"];
    
    CurrAssets->play(RESOURCE_FOLDER + "sounds/musicbox.flac");
}

/**
 * Draw the scene from the user's perspective
 */
void TitleState::renderScene(bool isMirror) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glCullFace(GL_BACK);
    
    updateViewMat();
    
    CurrAssets->lightingShader->startUsingShader();
    CurrAssets->lightingShader->setViewMatrix(viewMat);
    CurrAssets->lightingShader->setProjectionMatrix(perspectiveMat);
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->shadowMap_ID, 1);
    
	CurrAssets->lightingShader->setAttenuation(0.0002f);

    //	viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    lamp->draw(light);
    tv->draw(light);
    door->draw(light);
    
    shadowfbo->unbindTexture();
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
    title->draw(light);
    playGhost->draw(light);
    play->draw(light);
    
    // check OpenGL error
}

/**
 * Check the user's mouse to see if they're mousing over either "PLAY"
 *  button
 */
GameState *nextState;
void TitleState::update() {
    vec2 mousePos = titleControl();
    if (coordsOverPlay(mousePos.x, mousePos.y)) {
        play->material[0].diffuse[1] = 0.0;
        play->material[0].diffuse[2] = 0.0;
    }
    else {
        play->material[0].diffuse[1] = 255.0;
        play->material[0].diffuse[2] = 255.0;
    }
    
    if (coordsOverGhost(mousePos.x, mousePos.y)) {
        playGhost->material[0].diffuse[1] = 0.0;
        playGhost->material[0].diffuse[2] = 0.0;
    }
    else {
        playGhost->material[0].diffuse[1] = 255.0;
        playGhost->material[0].diffuse[2] = 255.0;
    }
    
    if (shouldStartParalyzed()) {
        shouldSwitch = true;
        nextState = new ParalyzedState(window);
        CurrAssets->stopSounds();
    }
    
    if (shouldStartGhost()) {
        shouldSwitch = true;
        nextState = new GhostState(window);
        CurrAssets->currShader = CurrAssets->ghostShader;
        CurrAssets->stopSounds();
    }
}


GameState* TitleState::newState() {
    return nextState;
}
