#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "glm/gtx/random.hpp"
#include "control.hpp"
#include "network.h"

using namespace glm;


GameState* GameState::newState() {
    printf("THIS SHOULD NOT BE CALLED LIKE EVER");
    return new GameState(window, false);
}

void GameState::initAssets() {
    Assets *assets = Assets::instance();
    
    bed =   assets->actorDictionary["bed"];
    clock = assets->actorDictionary["clock"];
    tv = assets->actorDictionary["tv"];
    lamp =  assets->actorDictionary["lamp-table"];
    room =  assets->actorDictionary["room"];
    enemy = assets->actorDictionary["enemy"];
    door = assets->actorDictionary["door"];
    fan = assets->actorDictionary["fan"];
    
    nightstand = assets->actorDictionary["nightstand"];
    doll = assets->actorDictionary["doll"];
    mirror = assets->actorDictionary["mirror"];
    chair = assets->actorDictionary["chair"];
    
    playerHealth = 100.0;
    
    Actor *tempCollectible = assets->actorDictionary["collect"];
    collectible = new Collectible(*tempCollectible);
    
    framebuffer = new Framebuffer();
    framebuffer->generate();
    framebuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    shadowfbo = new Framebuffer();
    shadowfbo->generate();
    shadowfbo->generateShadowTexture(4096, 4096);
    
    reflectbuffer = new Framebuffer();
    reflectbuffer->generate();
    reflectbuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    light = new Light();
    
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -10.0f, -10.0f,  0.0f,
        10.0f, -10.0f,   0.0f,
        -10.0f,  10.0f,  0.0f,
        -10.0f,  10.0f,  0.0f,
        10.0f, -10.0f,   0.0f,
        10.0f,  10.0f,   0.0f,
    };
    
    static const GLfloat g_quad_vertex_buffer_data_MIRROR[] = {
        -0.75f, .25f,  0.0f,
        0.75f,  .25f,   0.0f,
        -0.75f, 1.75f,  0.0f,
        -0.75f, 1.75f,  0.0f,
        0.75f,  .25f,   0.0f, 
        0.75f,  1.75f,   0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	flickerDuration = 0.0;
	flickerDirection = 1.0;
	attenFactor = 0.001;
	doorToggle = false;
	doorSlam = false;
	explodeDuration = 0.0;
	lampExplode = false;
	doorDirection = -1;
	shakeCamera = false;
    ghost_beat_player = false;
    player_beat_ghost = false;
    
    glGenBuffers(1, &quad_vertexbuffer_mirror);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer_mirror);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data_MIRROR), g_quad_vertex_buffer_data_MIRROR, GL_STATIC_DRAW);
    
    
    ghostWins = new HUDElement(RESOURCE_FOLDER + "hud/ghost_wins.png", 0.5, 0.5);
    playerWins = new HUDElement(RESOURCE_FOLDER + "hud/player_wins.png", 0.5, 0.5);
}

GameState::GameState(GLFWwindow *window_, bool isGhost_) {
    window = window_;
    isGhost = isGhost_;
    
    mirrorCamera = new Camera(vec3(0.0, 0.0, 6.0), vec3(0.0, 0.0, -1.0), 0.0, 0.0);
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
    
    updatePerspectiveMat();
    vf = new ViewFrustum();
    
    shouldSwitch = false;
    
    tvStaticDuration = 0;
    flickerDuration = 0;
    clockShakeDuration = 0;
}

/**
 * Called every frame yo
 */
void GameState::update() {
//    if (shouldWeReset()) {
//        playerHealth = 100.0;
//    }
    double currTime = glfwGetTime();
    elapsedTime = currTime - prevTime;
    prevTime = currTime;
    
    
    updateControl(window);
    updateCamDirection(camera);

	// interaction updates
	if (doorToggle) {
		updateDoorSwing();
	}
	if (doorSlam) {
		updateDoorSlam();
	}
	if (fanSpinDuration > 0.0f) {
		spinFan();
	}
    
    collectible->step(elapsedTime);
    
    if (tvStaticDuration > 0) {
        tvStaticDuration -= elapsedTime;
        if (tvStaticDuration < 0) {
            Actor *teevee = CurrAssets->actorDictionary["tv"];
            teevee->material[teevee->tvScreenIndex].ambient[0] = 0;
            teevee->material[teevee->tvScreenIndex].ambient[1] = 0;
            teevee->material[teevee->tvScreenIndex].ambient[2] = 0;
        }
    }
    
    Actor *clocky = CurrAssets->actorDictionary["clock"];
    if (clockShakeDuration > 0) {
        clockShakeDuration -= elapsedTime;
        clocky->direction.x = glm::linearRand(-0.9, 0.9);
        clocky->direction.y = glm::linearRand(-0.9, 0.9);
        clocky->direction.z = glm::linearRand(-0.9, 0.9);
    }
    else {
        clocky->direction = vec3(0, 0, 0);
    }
}

/**
 * Update the instance variable with the current view
 *  matrix
 */
void GameState::updateViewMat() {
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    
    mat4 mirrorCam = lookAt(mirrorCamera->center, mirrorCamera->center +
                                mirrorCamera->direction, vec3(0.0, 1.0, 0.0));
    
    viewMat = cam;
    mirrorViewMat = mirrorCam;
    //mirrorViewMat = cam;
}

void GameState::updateDoorSwing() {
	CurrAssets->actorDictionary["door"]->direction.y += doorDirection * 30.0f * elapsedTime * 0.30f;
    if (doorDirection > 0) {
		if (CurrAssets->actorDictionary["door"]->direction.y >= -10) {
			doorToggle = false;
			doorDirection = -1;
		}
	}
	else {
		if (CurrAssets->actorDictionary["door"]->direction.y <= -40) {
			doorToggle = false;
			doorDirection = 1;
		}
	}
}

void GameState::updateDoorSlam() {
    if (CurrAssets->actorDictionary["door"]->direction.y >= 0.0f) {
		CurrAssets->actorDictionary["door"]->direction.y = 0.0f;
		doorSlam = false;
		doorDirection = -1;
	}
	else {
		doorDirection = 1;
		CurrAssets->actorDictionary["door"]->direction.y += doorDirection * 200.0f * elapsedTime;
	}
}

void GameState::spinFan() {
	CurrAssets->actorDictionary["fan"]->direction.y += 150.0f * elapsedTime;
	fanSpinDuration -= elapsedTime;
}

void GameState::lightFlicker() {
	if (attenFactor > 0.02f) {
		flickerDirection = -1.0;
	}
	else if (attenFactor < 0.001) {
		flickerDirection = 1.0;
	}
	attenFactor = std::max(0.0005, attenFactor + flickerDirection * glm::compRand1(0.002f, 0.01f));

	flickerDuration = std::max(0.0, (flickerDuration - elapsedTime));
}

void GameState::lightExplode() {
	if (attenFactor > -0.0001) {
		attenFactor -= elapsedTime * glm::compRand1(0.0001f, 0.002f);
	}
	else {
		attenFactor = -1.0;
	}

	explodeDuration = std::max(0.0, (explodeDuration - elapsedTime));
	if (explodeDuration == 0.0) lampExplode = false;
}

/**
 * Update the instance variable with the current perspective
 *  matrix
 */
void GameState::updatePerspectiveMat() {
    mat4 Projection = perspective(FOV, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    perspectiveMat = Projection;
}

void GameState::updateHighlightMat() {
    Position playerLook = getPlayerLook();
	float yaw = playerLook.y, pitch = playerLook.x, hfov = playerLook.z;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    
    yaw = 180.0 / 3.141 * yaw;
    pitch = 180.0 / 3.141 * pitch;
    
    yaw += 180.0;
    
	glm::mat4 Ryaw     = glm::rotate(glm::mat4(1.0f), yaw, vec3(0, 1, 0));
    glm::mat4 Rpitch   = glm::rotate(glm::mat4(1.0f), pitch, vec3(1, 0, 0));
    glm::mat4 Trans    = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 Transform = Trans * Ryaw * Rpitch;
	glm::mat4 V = glm::inverse(Transform);
    
    // HACKY HACK HACK
	mat4 P = perspective(hfov, (float) (1920.0
                                            / 1080.0), 0.1f, 200.f);
	highlightVPMat = P * V;
}

void GameState::renderShadowBuffer() {
    shadowfbo->bind();

    glViewport(0, 0, 4096, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    CurrAssets->shadowShader->startUsingShader();
    
    bed->drawShadows(light);
    lamp->drawShadows(light);
    room->drawShadows(light);
    clock->drawShadows(light);
    tv->drawShadows(light);

    CurrAssets->shadowShader->disableAttribArrays();

    shadowfbo->unbind();
}

/**
 * Draw the specified actor iff it's within the user's view
 */
void GameState::viewFrustumCulling(Actor curActor){
    mat4 comboMatrix;
    
    comboMatrix = perspectiveMat * viewMat * curActor.modelMat;
    vf->extractPlanes(comboMatrix);
    
    int inView = vf->sphereIsInside(curActor.center, curActor.boundSphereRad);
    if (inView != OUTSIDE) {
//        curActor.draw(light);
        
    }
}

float coolTime = 0.0;

/**
 * Render a texture to a square that covers the whole screen.
 */
void GameState::renderFrameBuffer() {
    // Clear the screen
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(CurrAssets->currShader->fbo_ProgramID);
    framebuffer->bindTexture(CurrAssets->currShader->textureToDisplay_ID);
    
//    glUniform1f(CurrAssets->currShader->intensity_UniformID, 0.2);
    glUniform1f(CurrAssets->currShader->time_UniformID, coolTime);
    coolTime += 0.17;
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
                          CurrAssets->currShader->position_AttributeID, // attribute
                          3,                              // size
                          GL_FLOAT,                       // type
                          GL_FALSE,                       // normalized?
                          0,                              // stride
                          (void*)0                        // array buffer offset
                          );
    
    // Draw the triangles that cover the screen
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    
    glDisableVertexAttribArray(0);
    framebuffer->unbindTexture();
}

void GameState::renderReflectBuffer() {
    // Clear the screen
//    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    glUseProgram(CurrAssets->reflectionShader->reflection_ProgramID);
    reflectbuffer->bindTexture(CurrAssets->reflectionShader->reflection_sampler_ID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer_mirror);
    glVertexAttribPointer(
                          CurrAssets->reflectionShader->position_AttributeID, // attribute
                          3,                              // size
                          GL_FLOAT,                       // type
                          GL_FALSE,                       // normalized?
                          0,                              // stride
                          (void*)0                        // array buffer offset
                          );
    
    // Draw the triangles that cover the screenp
    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
    
    glDisableVertexAttribArray(0);
    reflectbuffer->unbindTexture();
}


void GameState::draw() {
   glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); // TODO: Turn this back on

    renderShadowBuffer();
    
    framebuffer->bind();
    renderScene(false);
    framebuffer->unbind();
    
    reflectbuffer->bind();
    renderScene(true);
    reflectbuffer->unbind();
    
    renderFrameBuffer();

    glDisable(GL_DEPTH_TEST);
    renderReflectBuffer();
    
    drawHUD();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    checkCollisions();
}


void GameState::drawHUD() {
    glDisable(GL_DEPTH_TEST);
    
    CurrAssets->hudShader->startUsingShader();
    CurrAssets->hudShader->setScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (ghost_beat_player) {
        ghostWins->drawElement(false);
    }
    
    if (player_beat_ghost) {
        playerWins->drawElement(false);
    }
}
