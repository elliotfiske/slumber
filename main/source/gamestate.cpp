#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"
#include "network.h"

using namespace glm;

float playerHealth = 1.2;

void GameState::initAssets() {
    Assets *assets = Assets::instance();
    
    bed =   assets->actorDictionary["bed"];
    clock = assets->actorDictionary["clock"];
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
    window = window_;
    isGhost = isGhost_;
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
    
    updatePerspectiveMat();
    vf = new ViewFrustum();
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

void GameState::renderShadowBuffer() {
    shadowfbo->bind();

    glViewport(0, 0, 2048, 2048);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    CurrAssets->shadowShader->startUsingShader();
    
    bed->drawShadows(light);
    lamp->drawShadows(light);
    room->drawShadows(light);
    clock->drawShadows(light);

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
    
    glUseProgram(CurrAssets->ghostShader->fbo_ProgramID);
    framebuffer->bindTexture(CurrAssets->ghostShader->textureToDisplay_ID);
    
    glUniform1f(CurrAssets->ghostShader->intensity_UniformID, 16.2);
    glUniform1f(CurrAssets->ghostShader->time_UniformID, coolTime);
    coolTime += 0.17;
    
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(
                          CurrAssets->ghostShader->position_AttributeID, // attribute
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
    
    checkCollisions();
}
/*
void GameState::billboardCheatSphericalBegin() {
   
   float modelview[16];
   int i,j;

   // save the current modelview matrix
   glPushMatrix();

   // get the current modelview matrix
   glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

   // undo all rotations
   // beware all scaling is lost as well 
   for( i=0; i<3; i++ ) 
       for( j=0; j<3; j++ ) {
      if ( i==j )
          modelview[i*4+j] = 1.0;
      else
          modelview[i*4+j] = 0.0;
       }

   // set the modelview with no rotations
   glLoadMatrixf(modelview);
}



void GameState::billboardEnd() {

   // restore the previously 
   // stored modelview matrix
   glPopMatrix();
}
*/