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
    ghostHealth = 100.0f;
    playerHealth = 100.0f;

	FOV = 35.0f;
	updatePerspectiveMat();
    camera = new Camera(vec3(0.0, 10.0, -6.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);

    mirrorCamera = new Camera(vec3(13.5, 0.0, -85.0), vec3(0.0, 1.0, 0.0), 0.0, 0.0);
    CurrAssets->lightingShader = CurrAssets->ghostLightingShader;
    CurrAssets->currFBOShader = CurrAssets->ghostShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    lampText = CurrAssets->billboardDictionary["lamp_tooltip.png"];
    
    ghostHUD = new HUDElement(RESOURCE_FOLDER + "hud/ghost_hud.png", 0.5, 0.5);
    ghostBar = new HUDElement(RESOURCE_FOLDER + "hud/ghost_health_hud.png", 0.5, 0.5);
    playerBar = new HUDElement(RESOURCE_FOLDER + "hud/player_health_hud.png", 0.5, 0.5);
    
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
void GhostState::renderScene(bool isMirror) {
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
    tv->draw(light, tvStaticDuration > 0.0);
	lamp->draw(light);
    door->draw(light);
    fan->draw(light);
    
    nightstand->draw(light);
    doll->draw(light);
    mirror->draw(light);
    chair->draw(light);


	shadowfbo->unbindTexture();
    
    CurrAssets->billboardShader->startUsingShader();
    CurrAssets->billboardShader->setViewMatrix(viewMat);
    CurrAssets->billboardShader->setProjectionMatrix(perspectiveMat);

    lampText->draw(light);
    
//	CurrAssets->collectibleShader->startUsingShader();
//	CurrAssets->collectibleShader->setViewMatrix(viewMat);
//	CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);


//	collectible->draw(light);

	CurrAssets->reflectionShader->startUsingShader();
	CurrAssets->reflectionShader->setViewMatrix(viewMat);
	
	shadowfbo->unbindTexture();

	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
//		cerr << "OpenGL error from Ghost state: " << err << endl;
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

void GhostState::updateCameraShake() {
	camera->direction.x += glm::compRand1(-0.5f, 0.5f) * elapsedTime;
	camera->direction.y += glm::compRand1(-0.5f, 0.5f) * elapsedTime;

	ghostHealth = fmaxf(-10.0, ghostHealth - 10.0 * elapsedTime);
}

void GhostState::update() {
    
    if (!player_beat_ghost) {
        ghostHealth += 0.027;
    }
    
    if (ghostHealth > 100.0) {
        ghostHealth = 100.;
    }
    
	GameState::update();
    viewFrustumCulling();

	if (shakeCamera) updateCameraShake();

	glm::vec3 lamppos  = CurrAssets->actorDictionary["lamp-table"]->center;
	glm::vec3 doorpos  = CurrAssets->actorDictionary["door"]->center;
    glm::vec3 clockpos = CurrAssets->actorDictionary["clock"]->center;
    glm::vec3 tvpos    = CurrAssets->actorDictionary["tv"]->center;

	if (checkBounds(lamppos - itemUseBounds, lamppos + itemUseBounds)) { /// Lamp action
		// Set billboard here!!

		if (getItemAction()) { // Flicker the light
			flickerDuration = 2.0;
            sendGhostAction(GHOST_ACTION_FLICKER_LAMP);
            
            CurrAssets->play(RESOURCE_FOLDER + "sounds/heartbeat.wav", lamp->center);
		}

		if (getItemAltAction()) {
			lampExplode = true;
			explodeDuration = 6.0;
			sendGhostAction(GHOST_ACTION_EXPLODE_LAMP);

			CurrAssets->play(RESOURCE_FOLDER + "sounds/glass-shatter.wav", vec3(50, 0, -70));
		}
	}
	else if (checkBounds(doorpos - itemUseBounds, doorpos + itemUseBounds)) { /// Door action
		// Set billboard here!!

		if (getItemAction()) { // Close/open door
			doorToggle = true;
            sendGhostAction(GHOST_ACTION_CREAK_DOOR);
            string two = creak1 ? "" : "2";
            CurrAssets->play(RESOURCE_FOLDER + "sounds/new_creak" + two + ".wav", door->center);
            creak1 = !creak1;
		}

		if (getItemAltAction()) {
		}
	}
	else if (checkBounds(clockpos - itemUseBounds, clockpos + itemUseBounds)) { /// Clock action
		// Set billboard here!!

		if (getItemAction()) { // Shake it
            clockShakeDuration = 3.0;
            sendGhostAction(GHOST_ACTION_POSSESS_CLOCK);
            CurrAssets->play(RESOURCE_FOLDER + "sounds/thump1.wav", clock->center);
		}
	}
    else if (checkBounds(tvpos - itemUseBounds, tvpos + itemUseBounds)) {
        
        if (getItemAction()) {
            tvStaticDuration = 1.8;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/tv_static.wav", tv->center);
            sendGhostAction(GHOST_ACTION_TV_STATIC);
        }
    }
    
    if (shouldWeReset()) {
        ghostHealth = 100.0f;
        playerHealth = 100.0f;
        
        player_beat_ghost = false;
        ghost_beat_player = false;
    }
    
    if (playerHealth < 0.0) {
        ghost_beat_player = true;
    }

    if (ghostHealth < 0.0 && !player_beat_ghost) {
        player_beat_ghost = true;
        sendGhostAction(GHOST_ACTION_LOST_HORRIBLY);
    }

	camera->step(elapsedTime, getForwardVelocity(), getStrafeVelocity());
	tellClientWhereGhostIs();
}

// check to see if ghost in inside the player's view 
void GhostState::viewFrustumCulling(){
    mat4 comboMatrix;
    
    comboMatrix = highlightVPMat * glm::translate(mat4(1.0), vec3(0.0, 0.0, 6.0));
    vf->extractPlanes(comboMatrix);
    
    int inView = vf->sphereIsInside(camera->center, 0.1f);
    if (inView != OUTSIDE && playerFOV < 28.0) {
		shakeCamera = true;
    }
	else {
		shakeCamera = false;
	}
}

void GhostState::damageGhost() {

}


void GhostState::gainHealth() {

}

float lastX, lastY, lastZ;

/**
 * Send a packet to the paralyzed player with the current
   location of the ghost
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
    
	ghostHUD->drawElement(true);
    
    CurrAssets->hudShader->setPercentShown(ghostHealth);
    ghostBar->drawElement(true);
    CurrAssets->hudShader->setPercentShown(getPlayerHealth());
    playerHealth = getPlayerHealth();
    playerBar->drawElement(true);
    
	CurrAssets->hudShader->setPercentShown(1000.0f);
}
