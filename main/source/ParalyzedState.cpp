//
//  ParalyzedState.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#include "ParalyzedState.h"
#include "network.h"

ParalyzedState::ParalyzedState(GLFWwindow *window): GameState(window, false) {
    camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
}

void ParalyzedState::checkCollisions() {
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
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID, 0);
    
    //    viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    tv->draw(light);
    lamp->draw(light);
    
    CurrAssets->collectibleShader->startUsingShader();
    CurrAssets->collectibleShader->setViewMatrix(viewMat);
    CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);
    
    collectible->draw(light);
    
    shadowfbo->unbindTexture();
    
    // check OpenGL error TODO: remove
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << endl;
    }
}
