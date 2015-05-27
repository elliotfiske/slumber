#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"
#include "network.h"

using namespace glm;

float playerHealth = 1.2;

GLuint generateMultiSampleTexture(GLuint samples) {
    GLuint texture;
    glGenTextures(1, &texture);
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, 100, 100, GL_TRUE);
    
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    
    return texture;
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
    // What enum to use?
    GLenum attachment_type;
    if(!depth && !stencil)
        attachment_type = GL_RGB;
    else if(depth && !stencil)
        attachment_type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        attachment_type = GL_STENCIL_INDEX;
    
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, WINDOW_WIDTH, WINDOW_HEIGHT, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
    else // Using both a stencil and depth test, needs special format arguments
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    return textureID;
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

    /** ANTI ALIASING */
//    framebuffer->bind();
//    antialiasTexture = generateMultiSampleTexture(4);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, antialiasTexture, 0);
//    framebuffer->unbind();
    
    shadowfbo = new Framebuffer();
    shadowfbo->generate();
    shadowfbo->generateShadowTexture(2048, 2048);
    
    
    // second framebuffer
    screenTexture = generateAttachmentTexture(false, false);
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// We only need a color buffer
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    

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