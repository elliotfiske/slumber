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
    camera = new Camera(vec3(0.0, 5.0, -65.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    title = CurrAssets->billboardDictionary["title.png"];
    play = CurrAssets->billboardDictionary["play.png"];
    playGhost = CurrAssets->billboardDictionary["play_ghost.png"];
    
    CurrAssets->play(RESOURCE_FOLDER + "/sounds/musicbox.flac");
}

/**
 * Draw the scene from the user's perspective
 */
void TitleState::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glCullFace(GL_BACK);
    
    updateViewMat();
    
    CurrAssets->lightingShader->startUsingShader();
    CurrAssets->lightingShader->setViewMatrix(viewMat);
    CurrAssets->lightingShader->setProjectionMatrix(perspectiveMat);
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID, 0);
    
    //	viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    lamp->draw(light);
    tv->draw(light);
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
    title->draw(light);
    play->draw(light);
    playGhost->draw(light);
    
    shadowfbo->unbindTexture();
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Title state: " << err << endl;
    }
}

/**
 * Check the user's mouse to see if they're mousing over either "PLAY"
 *  button
 */
GameState *nextState;
void TitleState::update() {
    vec2 mousePos = titleControl();
    if (mousePos.x < 570.0 && mousePos.x > 452.0 && mousePos.y < 470.0 && mousePos.y > 353.0) {
        play->material[0].diffuse[0] = 255.0;
    }
    else {
        play->material[0].diffuse[0] = 0.0;
    }
    
    if (mousePos.x < 570.0 && mousePos.x > 452.0 && mousePos.y < 600.0 && mousePos.y > 491.0) {
        playGhost->material[0].diffuse[0] = 255.0;
    }
    else {
        playGhost->material[0].diffuse[0] = 0.0;
    }
    
    if (shouldStartParalyzed()) {
        shouldSwitch = true;
        nextState = new ParalyzedState(window);
    }
    
    if (shouldStartGhost()) {
        shouldSwitch = true;
        nextState = new GhostState(window);
        CurrAssets->currShader = CurrAssets->ghostShader;
    }
}


GameState* TitleState::newState() {
    return nextState;
}
