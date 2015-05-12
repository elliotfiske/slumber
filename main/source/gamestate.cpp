#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"
#include "ViewFrustum.hpp"
#include "network.h"

using namespace glm;

bool collected1 = false;
bool collected2 = false;
bool collected3 = false;
bool collected4 = false;
bool collected5 = false;

float sphereTicks = 0;
float spookyspooky = 0;

float playerHealth = 1.2;

vector<vec3> spherePlaces;
ViewFrustum *vf;

void GameState::initAssets() {
    Assets *assets = Assets::instance();
    
    bed = assets->actorFromName("bed");
    clock = assets->actorFromName("clock");
    enemy = assets->actorFromName("enemy");
    lamp = assets->actorFromName("lamp-table");
    room = assets->actorFromName("room");
    
    sphere1 = assets->actorFromName("collect");
    sphere2 = assets->actorFromName("collect");
    sphere3 = assets->actorFromName("collect");
    sphere4 = assets->actorFromName("collect");
    sphere5 = assets->actorFromName("collect");
    
    spherePlaces.push_back(vec3(-40.6, 28.7, -103.52));
    spherePlaces.push_back(vec3(36.57, 26.8, -39.52));
    spherePlaces.push_back(vec3(-15.98, 9.77, -82.9));
    spherePlaces.push_back(vec3(-45.7, 8, -30.5));
    spherePlaces.push_back(vec3(21.7, 18.5, 0.5));
    
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
    if (isGhost_) {
        camera = new Camera(vec3(0.0, 5.0, -15.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    }
    else {
        camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
    }
    window = window_;
    isGhost = isGhost_;
    
    setupCallbacks(window);
    initAssets();
    
    prevTime = glfwGetTime();
}

int numCollected = 0;

void GameState::checkCollisions() {
    
    
    if (sphereTicks > 10) {
        if (!collected1) {
            collected1 = vf->gotLight(sphere1->center, 1.0);
            
            if (collected1) {
            numCollected++;
            }
        }
    }
    
    if (sphereTicks > 20) {
        if (!collected2) {
            collected2 = vf->gotLight(sphere2->center, 1.0);
            if (collected2) {
                numCollected++;
            }
        }
    }
    
    if (sphereTicks > 30) {
        if (!collected3) {
            collected3 = vf->gotLight(sphere3->center, 1.0);
            if (collected3) {
                numCollected++;
            }
        }
    }
    
    if (sphereTicks > 40) { 
        if (!collected4) {
            collected4 = vf->gotLight(sphere4->center, 1.0);
            if (collected4) {
                numCollected++;
            }
        }
    }
    
    if (sphereTicks > 50) {
        if (!collected5) {
            collected5 = vf->gotLight(sphere5->center, 1.0);
            if (collected5) {
                numCollected++;
            }
        }
    }
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
    if (playerHealth < 0) {
        playerHealth -= 0.009;
    }
    
    if (shouldWeReset()) {
        playerHealth = 100;
        sphereTicks = 0;
        spookyspooky = 0;
    }
    
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
        enemy->center.x = ghostPos.x;
        enemy->center.y = ghostPos.y;
        enemy->center.z = ghostPos.z;
    }
    
    prevTime = currTime;
    
    float enemyDist = distance(vec3(0, 0, 0), enemy->center);
    
    // Reduce by 0.1 if enemy is outside walls
    
    sphereTicks += 1/(enemyDist*enemyDist*25);
//    printf("Sphereticks: %f\n", sphereTicks);
    
    if (sphereTicks > 9999999999999) {
        sphereTicks = 0;
    }
}

void GameState::setView() {
    mat4 cam = lookAt(camera->center, camera->center
                                + camera->direction, vec3(0.0, 1.0, 0.0));
    
    viewMat = cam;
    CurrAssets->lightingShader->setViewMatrix(cam);
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
    mat4 Projection = perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    
    perspectiveMat = Projection;
    CurrAssets->lightingShader->setProjectionMatrix(Projection);
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
    enemy->drawShadows(light);

    CurrAssets->shadowShader->disableAttribArrays();

    shadowfbo->unbind();
}

void GameState::viewFrustumCulling(Actor curActor){
   vf = new ViewFrustum();
   mat4 comboMatrix;
   int result;
   
   setView();
   setPerspectiveMat();
   comboMatrix = perspectiveMat * viewMat * curActor.modelMat;
   vf->extractPlanes(comboMatrix);
   result = vf->sphereIsInside(curActor.center, curActor.boundSphereRad);
    if (!isGhost) {
        if(result == INSIDE || result == INTERSECT){
            //       printf("BOOGA BOOGA BOOGA\n");
            spookyspooky = 7;
            playerHealth -= 0.0035;
            printf("HP: %f\b", playerHealth);
        }
        else {
            //       printf("Naw man :(\n");
            spookyspooky = 0;
        }
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

    shadowfbo->bindTexture(CurrAssets->lightingShader->textureToDisplay_ID, 0);

    bed->draw(light);
    lamp->draw(light);
    room->draw(light);
    clock->draw(light);
    viewFrustumCulling(*enemy);
    
    if (sphereTicks > 10) {
        sphere1->center = spherePlaces[0];
        if (!collected1) {
            sphere1->draw(light);
        }
    }
    
    if (sphereTicks > 20) {
        sphere2->center = spherePlaces[1];
        if (!collected2) {
            sphere2->draw(light);
        }
    }
    
    if (sphereTicks > 30) {
        sphere3->center = spherePlaces[2];
        if (!collected3) {
            sphere3->draw(light);
        }
    }
    
    if (sphereTicks > 40) {
        sphere4->center = spherePlaces[3];
        if (!collected4) {
            sphere4->draw(light);
        }
    }
    
    if (sphereTicks > 50) {
        sphere5->center = spherePlaces[4];
        if (!collected5) {
            sphere5->draw(light);
        }
    }
    
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
    
    if (spookyspooky > 0 && playerHealth > 0) {
        glUseProgram(CurrAssets->motionBlurShader->fbo_ProgramID);
        framebuffer->bindTexture(CurrAssets->motionBlurShader->textureToDisplay_ID);
        
        CurrAssets->motionBlurShader->animateIntensity(spookyspooky/3, spookyspooky, currTime, 15);
    }
    else {
        glUseProgram(CurrAssets->darkeningShader->fbo_ProgramID);
        framebuffer->bindTexture(CurrAssets->darkeningShader->textureToDisplay_ID);
        
        CurrAssets->darkeningShader->setIntensity(2.0 * (1.2 - playerHealth));
    }
    
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
    
    checkCollisions();
}
