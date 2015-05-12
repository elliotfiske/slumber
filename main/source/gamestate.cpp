#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"
#include "ViewFrustum.hpp"
#include "network.h"

using namespace glm;

void GameState::initAssets() {
    Assets *assets = Assets::instance();
    
printf("LOAding assets\n");
    bed = assets->actorFromName("bed");
    clock = assets->actorFromName("clock");
    enemy = assets->actorFromName("enemy");
    lamp = assets->actorFromName("lamp-table");
    room = assets->actorFromName("room");
    
printf("Done LOAding assets\n");
    framebuffer = new Framebuffer();
    framebuffer->generate();
    framebuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    shadowfbo = new Framebuffer();
    shadowfbo->generate();
    shadowfbo->generateShadowTexture(2048, 2048);

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
    camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    
    window = window_;
    isGhost = isGhost_;
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
}

void GameState::checkCollisions() {
    // TODO: me!!!
}

float lastX, lastY, lastZ;

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

void GameState::update() {
    currTime = glfwGetTime();
    double elapsedTime = currTime - prevTime;
    
    updateControl(window);
    updateCamDirection(camera);
    updateLightPosition(light);
    
    if (isGhost) {
        camera->step(elapsedTime, getForwardVelocity(), getStrafeVelocity());
        tellClientWhereGhostIs();
    }
    else {
        Position ghostPos = getGhostPosition();
        printf("Ghost izzat: %f, %f, %f\n", ghostPos.x, ghostPos.y, ghostPos.z);
        enemy->center.x = ghostPos.x;
        enemy->center.y = ghostPos.y;
        enemy->center.z = ghostPos.z;
    }
    
    enemy->center.x += getForwardVelocity();
    enemy->center.y += getStrafeVelocity();
    
    float enemyYaw = atan2(enemy->center.y, enemy->center.x);
    float sqrtTerm = sqrt(enemy->center.x * enemy->center.x + enemy->center.y * enemy->center.y);
    float enemyPitch = atan2(sqrtTerm, enemy->center.z);
    
    printf("yawdiff: %f pitchdiff: %f\n", enemyYaw - getYaw() + 1.59, enemyPitch - getPitch() - 2.68);
    
    
    prevTime = currTime;
    
    checkCollisions();
}

void GameState::setView() {
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    
    this->viewMat = cam;
    CurrAssets->lightingShader->setViewMatrix(cam);
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
    mat4 Projection = perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    
    this->perspectiveMat = Projection;
    CurrAssets->lightingShader->setProjectionMatrix(Projection);
}



void GameState::renderShadowBuffer() {
    shadowfbo->bind();

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    CurrAssets->shadowShader->startUsingShader();
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    
    bed->drawShadows(light);
    lamp->drawShadows(light);
    room->drawShadows(light);
    clock->drawShadows(light);
    enemy->drawShadows(light);

    CurrAssets->shadowShader->disableAttribArrays();

    shadowfbo->unbind();
}

void GameState::viewFrustumCulling(Actor curActor){
   ViewFrustum *vf = new ViewFrustum();
   mat4 comboMatrix;
   int result;
   
   comboMatrix = this->perspectiveMat * this->viewMat * curActor.modelMat;
   vf->extractPlanes(comboMatrix, true);
   result = vf->sphereIsInside(curActor.center, 1);
   if(result == INSIDE || result == INTERSECT){
      curActor.draw(light);
   }
}

/**
 * Actually draws each of the 3D objects in the scene
 */
void GameState::renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    glCullFace(GL_BACK);
    
    CurrAssets->lightingShader->startUsingShader();
    setView();
    setPerspectiveMat();

    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID);
    
    bed->draw(light);
    lamp->draw(light);
    room->draw(light);
    clock->draw(light);
    enemy->draw(light);
    
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
    
    glUseProgram(CurrAssets->motionBlurShader->fbo_ProgramID);
    framebuffer->bindTexture(CurrAssets->motionBlurShader->textureToDisplay_ID);
    
    CurrAssets->motionBlurShader->animateIntensity(0, 10, currTime, 3);
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer( 
                          CurrAssets->motionBlurShader->position_AttributeID, // attribute
                          3,                              // size
                          GL_FLOAT,                       // type
                          GL_FALSE,                       // normalized?
                          0,                              // stride
                          (void*)0                        // array buffer offset
                          );
    
    // Draw the triangles that cover the screenp
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
