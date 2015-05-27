#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"
#include "network.h"

using namespace glm;

float playerHealth = 1.2;

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
    
    Actor *tempCollectible = assets->actorDictionary["collect"];
    collectible = new Collectible(*tempCollectible);
    
    framebuffer = new Framebuffer();
    framebuffer->generate();
    framebuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    shadowfbo = new Framebuffer();
    shadowfbo->generate();
    shadowfbo->generateShadowTexture(4096, 4096);
    

    light = new Light();
    
    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f,  0.0f,
        1.0f, -1.0f,   0.0f,
        -1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
        1.0f, -1.0f,   0.0f,
        1.0f,  1.0f,   0.0f,
    };
    
    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

GameState::GameState(GLFWwindow *window_, bool isGhost_) {
    window = window_;
    isGhost = isGhost_;
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
    
    updatePerspectiveMat();
    vf = new ViewFrustum();
    
    shouldSwitch = false;
}

/**
 * Called every frame yo
 */
void GameState::update() {
    if (shouldWeReset()) {
        playerHealth = 1.2;
    }
    
    double currTime = glfwGetTime();
    elapsedTime = currTime - prevTime;
    prevTime = currTime;
    
    updateControl(window);
    updateCamDirection(camera); 
    updateLightPosition(light);
    
    collectible->step(elapsedTime);
}

/**
 * Update the instance variable with the current view
 *  matrix
 */
void GameState::updateViewMat() {
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    
    viewMat = cam;
}

/**
 * Update the instance variable with the current perspective
 *  matrix
 */
void GameState::updatePerspectiveMat() {
    mat4 Projection = perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    perspectiveMat = Projection;
}

void GameState::updateHighlightMat() {
	float yaw = 0.0f, pitch = 0.0f;
	glm::vec3 position = glm::vec3(0.0f, 10.0f, -40.0f);

	glm::mat4 Ryaw     = glm::rotate(glm::mat4(1.0f), yaw, vec3(0, 1, 0));
    glm::mat4 Rpitch   = glm::rotate(glm::mat4(1.0f), pitch, vec3(1, 0, 0));
    glm::mat4 Trans    = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 Transform = Trans * Ryaw * Rpitch;
	glm::mat4 V = glm::inverse(Transform);
	mat4 P = perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
	highlightVPMat = P * V;
}

void GameState::renderShadowBuffer() {
    shadowfbo->bind();

    glViewport(0, 0, 4096, 4096);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        curActor.draw(light);
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
    
    glUniform1f(CurrAssets->currShader->intensity_UniformID, 0.2);
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


void GameState::draw() {
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);

    renderShadowBuffer();

    framebuffer->bind();
    renderScene();
    framebuffer->unbind();
    
    renderFrameBuffer();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    checkCollisions();
}


