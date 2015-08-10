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

float fadeInWordsTime = 0;

ParalyzedState::ParalyzedState(GLFWwindow *window): GameState(window, false) {
    playerHealth = 100;
    playerSensitivity = false;
	FOV = 27.0f;
	updatePerspectiveMat();
    camera = new Camera(vec3(0.0, 0.0, 6.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    mirrorCamera = new Camera(vec3(0.0, -2.2, -80.0), vec3(0.0, 0.0, 1.0), 0.0, 1.0);
	CurrAssets->lightingShader = CurrAssets->lightingShader;
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    introText = new HUDElement(RESOURCE_FOLDER + "hud/intro_text.png", 0.5, 0.5);
    
    numTimesPressedSpace = 0;
    
    timeToShowIntro = 10.0;
    
#ifdef THREADS
    thread *t1;
    
    t1 = new thread(doClientNetworking);
#endif
    
    
}

void ParalyzedState::checkCollisions() {
    mat4 comboMatrix;
    
    comboMatrix = perspectiveMat * viewMat * collectible->modelMat;
    vf->extractPlanes(comboMatrix);
    
    if (vf->gotLight(collectible->center, 5.0) && !ghost_beat_player) {
        collectible->collected();
        increaseHealth(10);
    }
}

float hurtCooldown;

void ParalyzedState::checkHurt(Actor *danger, int howMuch) {
    if (hurtCooldown > 0.0) {
    	return;
    }

	mat4 comboMatrix;
    
    comboMatrix = perspectiveMat * GameState::viewMat * danger->modelMat;
    vf->extractPlanes(comboMatrix);
    
    int inView = vf->sphereIsInside(danger->center, danger->boundSphereRad * 3.0);
    if (inView != OUTSIDE) {
        
        if (getParalyzedZoom()) {
            howMuch *= 2;
        }
        
        lowerHealth(howMuch);
        hurtCooldown = 5.0;
    }
}

void ParalyzedState::checkZoom() {
	if (getParalyzedZoom() == true) {
		float diff = 15.0f - FOV;
		FOV += diff / 3;
	//	FOV = fmaxf(15.0f, FOV - elapsedTime * 15.0f * 4.0f);
		updatePerspectiveMat();
	}
	else if (FOV < 27.0f) {
		FOV = fminf(27.0f, FOV + elapsedTime * 15.0f * 4.0);
		updatePerspectiveMat();
	}

	redness = (27.0f - FOV) / (27.0f - 15.0f);
    CurrAssets->currShader->setDarknessModifier(redness);
}

bool creakOne = true;


void ParalyzedState::update() {
    GameState::update();
//    sf::Listener::setDirection(camera->direction.x, camera->direction.y, camera->direction.z);
    
    hurtCooldown -= 0.17;
    timeToShowIntro -= 0.02;
    if (getParalyzedZoom()) {
        timeToShowIntro -= 0.1;
    }
    
//    CurrAssets->play(RESOURCE_FOLDER + "sounds/tv_static.wav", vec3(10, 0, 0)); // TODO: DELETE

    int currAction = actionReady();
    if (currAction) {
        if (currAction == GHOST_ACTION_CREAK_DOOR) {
            doorToggle = true;
            
            string two = creakOne ? "" : "2";
            CurrAssets->play(RESOURCE_FOLDER + "sounds/new_creak" + two + ".wav", door->center);
            creakOne = !creakOne;
            
            checkHurt(door, 5);
        }
        
        if (currAction == GHOST_ACTION_FLICKER_LAMP) {
            CurrAssets->play(RESOURCE_FOLDER + "sounds/heartbeat.wav", lamp->center);
            
            flickerDuration = 2.0;
            checkHurt(lamp, 7);
        }
        
        if (currAction == GHOST_ACTION_POSSESS_CLOCK) {
            clockShakeDuration = 3.0;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/thump1.wav", clock->center);
            
            checkHurt(clock, 5);
        }
        
        if (currAction == GHOST_ACTION_TV_STATIC) {
            tvStaticDuration = 1.8;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/tv_static.wav", tv->center);
            checkHurt(tv, 8);
        }
        
        if (currAction == GHOST_ACTION_EXPLODE_LAMP) {
            explodeDuration = 6.0;
            lampExplode = true;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/glass-shatter.wav", vec3(50, 0, -70));
            checkHurt(lamp, 10);
        }
        
        if (currAction == GHOST_ACTION_LOST_HORRIBLY) {
            player_beat_ghost = true;
        }
        
        if (currAction == GHOST_ACTION_SLAM_DOOR) {
            CurrAssets->play(RESOURCE_FOLDER + "sounds/door-slam.wav", door->center);
            
            doorSlam = true;
            checkHurt(door, 9);
        }
        
        if (currAction == GHOST_ACTION_SPIN_FAN) {
            CurrAssets->play(RESOURCE_FOLDER + "sounds/spinning.wav", fan->center);
            
            fanSpinDuration = 9.0;
            checkHurt(fan, 7);
        }
        
        if (currAction == GHOST_ACTION_GLOW_DOLL) {
            dollGlowDuration = 3.0;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/doll_laugh.wav", tv->center);
            
            checkHurt(doll, 10);
        }
        
        if (currAction == GHOST_ACTION_MOVE_DOLL) {
            dollMoveDuration = 3.9;
            CurrAssets->play(RESOURCE_FOLDER + "sounds/doll_laugh.wav", tv->center);
            
            checkHurt(doll, 10);
        }

		if (currAction == GHOST_ACTION_SHORT_FAN) {
            CurrAssets->play(RESOURCE_FOLDER + "sounds/spinning.wav", fan->center);
            
			fanShakeDuration = 3.0;
			checkHurt(fan, 15);
        }
    }
    
    int newSpaces = numSpaces();
    if (newSpaces > numTimesPressedSpace) {
        numTimesPressedSpace = newSpaces;
        
        fadeInWordsTime = 100.0;
    }
    
    fadeInWordsTime -= 1.0;
    if (fadeInWordsTime < 0) {
        fadeInWordsTime = 0;
    }
    
    Position ghostPos = getGhostPosition();
    enemy->center.x = ghostPos.x;
    enemy->center.y = ghostPos.y;
    enemy->center.z = ghostPos.z;
    
    viewFrustumCulling();
    tellGhostWhereImLooking();
    darkness = (100 - playerHealth) * 4.0 / 100;
    CurrAssets->currShader->setIntensity(darkness);    


	checkZoom();

    if (shouldWeReset()) {
        playerHealth = 100.0;
    printf("resetting lol\n"); 
        player_beat_ghost = false;
        ghost_beat_player = false;
    }
}


/**
 * Send a packet to the ghost player with the current
 *  location of where I'm looking
 */
void ParalyzedState::tellGhostWhereImLooking() {
#ifdef THREADS
	float pitch = getPitch();
	float yaw = getYaw();

    sendPlayerLook(pitch, yaw, FOV, (float) playerHealth);
#endif
}

/**
 * Draw the scene from the user's perspective
 */
void ParalyzedState::renderScene(bool isMirror) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glCullFace(GL_BACK);
    
    updateViewMat();

    CurrAssets->lightingShader->startUsingShader();
    CurrAssets->lightingShader->setViewMatrix(isMirror ? mirrorViewMat : viewMat);
    CurrAssets->lightingShader->setProjectionMatrix(perspectiveMat);
	CurrAssets->lightingShader->setHighlightVP(highlightVPMat);
    
    shadowfbo->bindTexture(CurrAssets->lightingShader->shadowMap_ID, 1);
    
    if (playerHealth < 0.0) {
        ghost_beat_player = true;
    }

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
    
    //    viewFrustumCulling(*bed);
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    tv->draw(light, tvStaticDuration > 0.0);
    lamp->draw(light);
    door->draw(light);
    fan->draw(light);
    
    nightstand->draw(light);
    doll->draw(light);
    chair->draw(light);
    
    shadowfbo->unbindTexture();

    CurrAssets->collectibleShader->startUsingShader();
    CurrAssets->collectibleShader->setViewMatrix(viewMat);
    CurrAssets->collectibleShader->setProjectionMatrix(perspectiveMat);
    
    if (FOV < 26.0) {
    	enemy->draw(light);
    }
    
   CurrAssets->reflectionShader->startUsingShader();
   CurrAssets->reflectionShader->setViewMatrix(viewMat);
    
   glm::mat4 mirror_translation = glm::translate(glm::mat4(1.0f), vec3(0, 0, 100));
   CurrAssets->reflectionShader->setModelMatrix(glm::mat4(1.0));
   CurrAssets->reflectionShader->setProjectionMatrix(perspectiveMat);
    
    // check OpenGL error TODO: remove
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
      //  cerr << "OpenGL error: " << err << endl;
    }
}
void ParalyzedState::increaseHealth(int healthValue){
    playerHealth += healthValue;
    if (playerHealth > 100) {
        playerHealth = 100;
    }
}
void ParalyzedState::lowerHealth(int severity){
   if(playerSensitivity == false) {
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
    
bool ParalyzedState::getSensitivity() {
   return playerSensitivity;
}

void ParalyzedState::drawHUD() {
    GameState::drawHUD();
    
    if (timeToShowIntro > 0.0) {
        introText->drawElement(false);
    }
    
    CurrAssets->hudShader->setPercentShown(1000.0f);
}

void ParalyzedState::viewFrustumCulling(){
    mat4 comboMatrix;
    
    comboMatrix = perspectiveMat * viewMat * glm::translate(mat4(1.0), vec3(0.0, 0.0, 0.0));
    vf->extractPlanes(comboMatrix);
    
    int inView = vf->sphereIsInside(enemy->center, 0.1f);
    if (inView != OUTSIDE && playerFOV < 26.0) {
      printf("Enemy in sight\n");
	   CurrAssets->play(RESOURCE_FOLDER + "sounds/gross-static.wav", enemy->center);
    }
    else{
      printf("Enemy not in sight\n");
    }
}
