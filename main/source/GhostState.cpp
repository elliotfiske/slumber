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
#include "glm/gtx/random.hpp"

#ifdef THREADS
    #include <thread>
#endif

GhostState::GhostState(GLFWwindow *window) : GameState(window, true) {
    camera = new Camera(vec3(0.0, 5.0, -15.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    mirrorCamera = new Camera(vec3(13.5, 0.0, -85.0), vec3(0.0, 1.0, 0.0), 0.0, 0.0);
    CurrAssets->lightingShader = CurrAssets->ghostLightingShader;
    CurrAssets->currFBOShader = CurrAssets->ghostShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    lampText = CurrAssets->billboardDictionary["lamp_tooltip.png"];
    
#ifdef THREADS
    thread *t1;
    
    t1 = new thread(doGhostNetworking);
#endif
	itemUseBounds = glm::vec3(25.0f, 25.0f, 25.0f);
}

void GhostState::checkCollisions() {
	// TODO: make sure ghost can't go through stuff
}

void GhostState::lightFlicker() {
	if (attenFactor > 0.02f) {
		flickerDirection = -1.0;
	}
	else if (attenFactor < 0.001) {
		flickerDirection = 1.0;
	}
	attenFactor = std::max(0.0005, attenFactor + flickerDirection * glm::compRand1(0.002f, 0.01f));
	CurrAssets->lightingShader->setAttenuation(attenFactor);

	flickerDuration = std::max(0.0, (flickerDuration - elapsedTime));
}

/**
 * Draw the scene from the user's perspective
 */
void GhostState::renderScene(bool isMirror) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glDisable(GL_CULL_FACE);

	updateViewMat();
	updateHighlightMat();

	CurrAssets->ghostLightingShader->startUsingShader();
	CurrAssets->ghostLightingShader->setViewMatrix(viewMat);
	CurrAssets->ghostLightingShader->setProjectionMatrix(perspectiveMat);
	CurrAssets->ghostLightingShader->setHighlightVP(highlightVPMat);

	shadowfbo->bindTexture(CurrAssets->ghostLightingShader->shadowMap_ID, 1);

	if (flickerDuration > 0.0) {
		lightFlicker();
	}
	else {
		CurrAssets->lightingShader->setAttenuation(0.0002f);
	}

    bed->draw(light);
	room->draw(light);
	clock->draw(light);
    tv->draw(light, tvStaticDuration > 0.0);
	lamp->draw(light);
    door->draw(light);
    fan->draw(light);

	shadowfbo->unbindTexture();
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);
    
    lampText->draw(light);
    
	CurrAssets->collectibleShader->startUsingShader();
	CurrAssets->collectibleShader->setViewMatrix(viewMat);
	CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);

	collectible->draw(light);

	CurrAssets->reflectionShader->startUsingShader();
	CurrAssets->reflectionShader->setViewMatrix(viewMat);
    
    glm::mat4 mirror_translation = glm::translate(glm::mat4(1.0f), vec3(0, 0, -1));
    CurrAssets->reflectionShader->setModelMatrix(glm::mat4(1.0f));
	CurrAssets->reflectionShader->setProjectionMatrix(glm::mat4(1.0f));
	
	shadowfbo->unbindTexture();

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

bool creak1 = true;

void GhostState::update() {
	GameState::update();

	glm::vec3 lamppos  = CurrAssets->actorDictionary["lamp-table"]->center;
	glm::vec3 doorpos  = CurrAssets->actorDictionary["door"]->center;
    glm::vec3 clockpos = CurrAssets->actorDictionary["clock"]->center;
    glm::vec3 tvpos    = CurrAssets->actorDictionary["tv"]->center;

	if (checkBounds(lamppos - itemUseBounds, lamppos + itemUseBounds)) { /// Lamp action
		// Set billboard here!!

		if (getItemAction()) { // Flicker the light
			flickerDuration = 2.0;
            sendGhostAction(GHOST_ACTION_FLICKER_LAMP);
            
            CurrAssets->play(RESOURCE_FOLDER + "sounds/heartbeat.wav");
		}
	}
	else if (checkBounds(doorpos - itemUseBounds, doorpos + itemUseBounds)) { /// Door action
		// Set billboard here!!

		if (getItemAction()) { // Close/open door
			doorToggle = true;
            sendGhostAction(GHOST_ACTION_CREAK_DOOR);
            string two = creak1 ? "" : "2";
            CurrAssets->play(RESOURCE_FOLDER + "sounds/new_creak" + two + ".wav");
            creak1 = !creak1;
		}
	}
	else if (checkBounds(clockpos - itemUseBounds, clockpos + itemUseBounds)) { /// Clock action
		// Set billboard here!!

		if (getItemAction()) { // Shake it
            clockShakeDuration = 3.0;
            sendGhostAction(GHOST_ACTION_POSSESS_CLOCK);
            CurrAssets->play(RESOURCE_FOLDER + "sounds/thump1.wav");
		}
	}
    else if (checkBounds(tvpos - itemUseBounds, tvpos + itemUseBounds)) {
        
        if (getItemAction()) {
            tvStaticDuration = 1.8;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/tv_static.wav");
            sendGhostAction(GHOST_ACTION_TV_STATIC);
        }
    }
    else if (shouldWeReset()) {
        
        sendGhostAction(GHOST_ACTION_BOO);
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

void GhostState::drawHUD() {
    GameState::drawHUD();
    
    
}
