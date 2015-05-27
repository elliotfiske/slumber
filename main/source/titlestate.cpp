//
//  titlestate.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/15/15.
//
//

#include "titlestate.h"
#include "control.hpp"

TitleState::TitleState(GLFWwindow *window): GameState(window, false) {
    camera = new Camera(vec3(0.0, 5.0, -95.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
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
    tv->draw(light);
    lamp->draw(light);
    
    CurrAssets->collectibleShader->startUsingShader();
    CurrAssets->collectibleShader->setViewMatrix(viewMat);
    CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);
    
//	collectible->draw(light);
    
    shadowfbo->unbindTexture();
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Ghost state: " << err << endl;
    }
}

/**
 * Check the user's mouse to see if they're mousing over either "PLAY"
 *  button
 */
void TitleState::update() {
    // DON'T CALL super.update() plz
    updateCamDirection(camera);
//    updateLightPosition(light);
}