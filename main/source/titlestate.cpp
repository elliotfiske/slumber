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
    
    CurrAssets->play(RESOURCE_FOLDER + "/sounds/musicbox.flac");
    
    
    Actor *tempCollectible = CurrAssets->actorDictionary["collect"];
    button1 = new Collectible(*tempCollectible);
    button1->center = vec3(0.0, 4.0, -100.0);
    
    button2 = new Collectible(*tempCollectible);
    button2->center = vec3(0.0, -1.0, -100.0);
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
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->shadowMap_ID, 1);
    
    //	viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    lamp->draw(light);
    tv->draw(light);
    
    shadowfbo->unbindTexture();

    CurrAssets->collectibleShader->startUsingShader();
    CurrAssets->collectibleShader->setViewMatrix(viewMat);
    CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);
    
    button1->draw(light);
    button2->draw(light);
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
    title->material[0].diffuse[0] = 1.0;
    title->draw(light);
    
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
        button1->red = true;
    }
    else {
        button1->red = false;
    }
    
    if (mousePos.x < 570.0 && mousePos.x > 452.0 && mousePos.y < 600.0 && mousePos.y > 491.0) {
        button2->red = true;
    }
    else {
        button2->red = false;
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
