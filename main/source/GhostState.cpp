//
//  GhostState.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#include "GhostState.h"
#include "control.hpp"
#include "network.h"

GhostState::GhostState(GLFWwindow *window) :
GameState(window, true) {
    camera = new Camera(vec3(0.0, 5.0, -15.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    CurrAssets->lightingShader = CurrAssets->ghostLightingShader;
    CurrAssets->currFBOShader = CurrAssets->ghostShader;
//    CurrAssets->ghostShader = CurrAssets->motionBlurShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    title = CurrAssets->billboardDictionary["title.png"];
}

void GhostState::checkCollisions() {
	// TODO: make sure ghost can't go through stuff
}

/**
 * Draw the scene from the user's perspective
 */
void GhostState::renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	glCullFace(GL_BACK);

	updateViewMat();
	updateHighlightMat();

	CurrAssets->ghostLightingShader->startUsingShader();
	CurrAssets->ghostLightingShader->setViewMatrix(viewMat);
	CurrAssets->ghostLightingShader->setProjectionMatrix(perspectiveMat);
	CurrAssets->ghostLightingShader->setHighlightVP(highlightVPMat);

	shadowfbo->bindTexture(CurrAssets->ghostLightingShader->textureToDisplay_ID, 8);

    bed->draw(light);
	room->draw(light);
	clock->draw(light);
    tv->draw(light);
	lamp->draw(light);

	shadowfbo->unbindTexture();
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
    title->draw(light);
    

	CurrAssets->collectibleShader->startUsingShader();
	CurrAssets->collectibleShader->setViewMatrix(viewMat);
	CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);

//	collectible->draw(light);

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error from Ghost state: " << err << endl;
	}
}

void GhostState::update() {
	GameState::update();

	camera->step(elapsedTime, getForwardVelocity(), getStrafeVelocity());
	tellClientWhereGhostIs();
}

float lastX, lastY, lastZ;

/**
 * Send a packet to the paralyzed player with the current
 *  location of the ghost
 */
void GameState::tellClientWhereGhostIs() {
#ifdef THREADS
	float x = camera->center.x;
	float y = camera->center.y;
	float z = camera->center.z;

	if (lastX != x || lastY != y || lastZ != z) {
		lastX = camera->center.x;
		lastY = camera->center.y;
		lastZ = camera->center.z;
		sendGhostPosition(lastX, lastY, lastZ);
	}
#endif
}
