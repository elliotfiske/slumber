#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"

using namespace glm;

void GameState::initAssets() {
    Assets *assets = Assets::instance();
    room = assets->actorFromName("room");
    room->diffuseColor = vec3(0.31, 0.082, 0.212);
    room->ambientColor = vec3(1.0, 0.05, 0.3);
    room->specularColor = vec3(0.1, 0.1, 0.1);
    room->shininess = 0;
    
    bed = assets->actorFromName("sheet");
    bed->diffuseColor = vec3(0.1, 0.2, 0.3);
    bed->ambientColor = vec3(0.15, 0.06, 0.07);
    bed->specularColor = vec3(0.1, 0.1, 0.1);
    bed->shininess = 20;
    
    clock = assets->actorFromName("clock");
    clock->diffuseColor = vec3(0.388, 0.231, 0.102);
    clock->ambientColor = vec3(0.1, 0.06, 0.17);
    clock->specularColor = vec3(0.1, 0.1, 0.1);
    clock->shininess = 10;
    
    framebuffer = new Framebuffer();
    framebuffer->generate();
    framebuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    shadowfbo = new Framebuffer();
    shadowfbo->generate();
    shadowfbo->generateTexture(2048, 2048);
    
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

GameState::GameState(GLFWwindow *window_) {
    camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    
    window = window_;
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
}

void GameState::checkCollisions() {
    // TODO: meb
}

void GameState::update() {
    double curTime = glfwGetTime();
    double elapsedTime = curTime - prevTime;
    
    updateControl(window);
    updateCamDirection(camera);
    camera->step(elapsedTime, getForwardVelocity(), getStrafeVelocity());
    
    prevTime = curTime;
    
    checkCollisions();
}

void GameState::setView() {
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    CurrAssets->lightingShader->setViewMatrix(cam);
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
    mat4 Projection = perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    CurrAssets->lightingShader->setProjectionMatrix(Projection);
}



void GameState::renderShadowBuffer() {
    shadowfbo->bind();

    glViewport(0, 0, 2048, 2048);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    CurrAssets->shadowShader->startUsingShader();

    bed->drawShadows(light);
    room->drawShadows(light);
    clock->drawShadows(light);

    CurrAssets->shadowShader->disableAttribArrays();

    shadowfbo->unbind();
}

/**
 * Actually draws each of the 3D objects in the scene
 */
void GameState::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    CurrAssets->lightingShader->startUsingShader();
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    
    setView();
    setPerspectiveMat();

    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID);
    
    bed->draw(light);
    room->draw(light);
    clock->draw(light);
    
    CurrAssets->lightingShader->disableAttribArrays();
    shadowfbo->unbindTexture();
}

/**
 * Render a texture to a square that covers the whole screen.
 */
void GameState::renderFrameBuffer() {
    // Clear the screen
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(CurrAssets->darkeningShader->fbo_ProgramID);
    framebuffer->bindTexture(CurrAssets->darkeningShader->textureToDisplay_ID);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
                          CurrAssets->darkeningShader->position_AttributeID, // attribute
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
	glEnable(GL_CULL_FACE);

    renderShadowBuffer();

    framebuffer->bind();
    renderScene();
    framebuffer->unbind();
    
    renderFrameBuffer();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}
