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

#ifdef THREADS
    #include <thread>
#endif

GhostState::GhostState(GLFWwindow *window) :
GameState(window, true) {
    camera = new Camera(vec3(0.0, 5.0, -15.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    CurrAssets->lightingShader = CurrAssets->ghostLightingShader;
    CurrAssets->currFBOShader = CurrAssets->ghostShader;
//    CurrAssets->ghostShader = CurrAssets->motionBlurShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    lampText = CurrAssets->billboardDictionary["lamp_tooltip.png"];
    
#ifdef THREADS
    thread *t1;
    
    t1 = new thread(doGhostNetworking);
#endif
	itemUseBounds = glm::vec3(30.0f, 30.0f, 30.0f);
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
    glDisable(GL_CULL_FACE);

	updateViewMat();
	updateHighlightMat();

	CurrAssets->lightingShader->startUsingShader();
	CurrAssets->lightingShader->setViewMatrix(viewMat);
	CurrAssets->lightingShader->setProjectionMatrix(perspectiveMat);
	CurrAssets->lightingShader->setHighlightVP(highlightVPMat);

	shadowfbo->bindTexture(CurrAssets->lightingShader->shadowMap_ID, 1);

	if (lampExplode) {
		lightExplode();
	}
	else if (flickerDuration > 0.0) {
		lightFlicker();
	}
	else {
		attenFactor = 0.0002f;
	}
	CurrAssets->lightingShader->setAttenuation(attenFactor);

    bed->draw(light);
	room->draw(light);
	clock->draw(light);
    tv->draw(light);
	lamp->draw(light);
    door->draw(light);

	shadowfbo->unbindTexture();
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
//    lampText->draw(light);
    
//	CurrAssets->collectibleShader->startUsingShader();
//	CurrAssets->collectibleShader->setViewMatrix(viewMat);
//	CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);

//	collectible->draw(light);

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		cerr << "OpenGL error from Ghost state: " << err << endl;
	}
}

// Check player position against a bounding box
bool GhostState::checkBounds(glm::vec3 min, glm::vec3 max) {
	glm::vec3 pos = camera->center;
	if (pos.x > min.x && pos.x < max.x &&
		pos.y > min.y && pos.y < max.y &&
		pos.z > min.z && pos.z < max.z) {
		return true;
	}
	return false;
}

void GhostState::update() {
	GameState::update();

	if (shakeCamera) updateCameraShake();

	glm::vec3 lamppos = CurrAssets->actorDictionary["lamp-table"]->center;
	glm::vec3 doorpos = CurrAssets->actorDictionary["door"]->center;
	glm::vec3 clockpos = CurrAssets->actorDictionary["clock"]->center;

	if (checkBounds(lamppos - itemUseBounds, lamppos + itemUseBounds)) { /// Lamp action
		// Set billboard here!!

		if (getItemAction()) { // Flicker the light
			flickerDuration = 2.0;
		}

		if (getItemAltAction()) {
			lampExplode = true;
			explodeDuration = 8.0;
		}
	}
	else if (checkBounds(doorpos - itemUseBounds, doorpos + itemUseBounds)) { /// Door action
		// Set billboard here!!

		if (getItemAction()) { // Close/open door
			doorToggle = true;
		}

		if (getItemAltAction()) {
			shakeCamera = true;
		}
	}
	else if (checkBounds(clockpos - itemUseBounds, clockpos + itemUseBounds)) { /// Clock action
		// Set billboard here!!

		if (getItemAction()) { // Shake it
		}
	}


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
