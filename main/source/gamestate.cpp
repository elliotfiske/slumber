#include "gamestate.hpp"
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include "glm/gtc/random.hpp"
#include "control.hpp"

void GameState::initAssets() {
    
    groundPlane = new Actor(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 0.0, 0.0);
    groundPlane->posID = assets.pos_roomID;
    groundPlane->norID = assets.nor_roomID;
    groundPlane->indID = assets.ind_roomID;
    groundPlane->numVerts = assets.numVerts_room;
    
    groundPlane->diffuseColor = vec3(0.31, 0.082, 0.212);
    groundPlane->ambientColor = vec3(1.0, 0.05, 0.3);
    groundPlane->specularColor = vec3(0.1, 0.1, 0.1);
    groundPlane->shininess = 0;
    
    bed = new Actor(vec3(0.0, -1.0, 0.0), vec3(0.0, 0.0, 0.0), 0.0, 0.0);
    bed->posID = assets.pos_bedID;
    bed->norID = assets.nor_bedID;
    bed->indID = assets.ind_bedID;
    bed->numVerts = assets.numVerts_bed;
    
    bed->diffuseColor = vec3(0.1, 0.2, 0.3);
    bed->ambientColor = vec3(0.15, 0.06, 0.07);
    bed->specularColor = vec3(0.1, 0.1, 0.1);
    bed->shininess = 20;
    
    for(int i = 0; i < 5; i++){
        clock = new Actor(vec3(12.5, -2.0, 0.0), vec3(0.0, 180.0, 0.0), 0.0, 0.0);
        clock->posID = assets.pos_clockID;
        clock->norID = assets.nor_clockID;
        clock->indID = assets.ind_clockID;
        clock->numVerts = assets.numVerts_clock;
        
        clock->diffuseColor = vec3(0.388, 0.231, 0.102);
        clock->ambientColor = vec3(0.1, 0.06, 0.17);
        clock->specularColor = vec3(0.1, 0.1, 0.1);
        clock->shininess = 10;
        
        clocks.push_back(*clock);
    }
    
    clocks[0].center.x = 12.5;
    clocks[0].center.y = -2.0;
    clocks[0].center.z = 0.0;
    
    clocks[1].center.x = -12.5;
    clocks[1].center.y = -2.0;
    clocks[1].center.z = 0.0;
    
    clocks[2].center.x = 12.5;
    clocks[2].center.y = -2.0;
    clocks[2].center.z = -32.5;
    
    clocks[3].center.x =  -12.5;
    clocks[3].center.y = -2.0;
    clocks[3].center.z =  -32.5;
    
    clocks[4].center.x = 0;
    clocks[4].center.y = -2.0;
    clocks[4].center.z = -42.5;
    
    
    
    framebuffer = new Framebuffer();
    framebuffer->generate();
    framebuffer->generateTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    
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
    timeToNextSphere = 0;
    numCurSpheres = 0;
    numSpheresHit = 0;
}

void GameState::spawnSphere() {
    float randX = glm::linearRand(-XMAX, XMAX);
    float randZ = glm::linearRand(-ZMAX, ZMAX);
    
    float randAngle = glm::linearRand(0.0, 2 * 3.14159);
    float randXVel = cos(randAngle);
    float randZVel = sin(randAngle);
    
    Actor *newActor = new Actor(vec3(randX, 0.2, randZ), vec3(randXVel, 0.0,
                                                              randZVel), 5.0, 1.0);
    newActor->posID = assets.pos_sphereID;
    newActor->norID = assets.nor_sphereID;
    newActor->indID = assets.ind_sphereID;
    newActor->numVerts = assets.numVerts_sphere;
    actors.push_back(*newActor);
    numCurSpheres++;
}

void GameState::checkCollisions() {
    
}

void GameState::update() {
    
    updateControl(window);
    updateCamera(camera);
    
    double curTime = glfwGetTime();
    double elapsedTime = curTime - prevTime;
    prevTime = curTime;
    for (int i = 0; i < actors.size(); i++) {
        actors[i].step(elapsedTime);
    }
    
    checkCollisions();
    
    camera->center += camera->direction * (float) elapsedTime
    * getForwardVelocity();
    glm::vec3 right = glm::cross(camera->direction, glm::vec3(0.0, 1.0, 0.0));
    camera->center += right * (float) elapsedTime * getStrafeVelocity();
    if(camera->center.y <= 0.0){
        camera->center.y = 0.0;
    }
    if(camera->center.y >= 10.4){
        camera->center.y = 10.4;
    }
    
    if(camera->center.x >= 13.5){
        camera->center.x = 13.5;
    }
    if(camera->center.x <= -13.5){
        camera->center.x = -13.5;
    }
    if(camera->center.z >= 2.5){
        camera->center.z = 2.5;
    }
    if(camera->center.z <= -46){
        camera->center.z = -46;
    }
}

void GameState::setView() {
    glm::mat4 cam = glm::lookAt(camera->center, camera->center
                                + camera->direction, glm::vec3(0.0, 1.0, 0.0));
    glUniformMatrix4fv(assets.h_uViewMatrix, 1, GL_FALSE, glm::value_ptr(cam));
}

/* helper function to set projection matrix - don't touch */
void GameState::setPerspectiveMat() {
    glm::mat4 Projection = glm::perspective(45.0f, (float) WINDOW_WIDTH
                                            / WINDOW_HEIGHT, 0.1f, 200.f);
    glUniformMatrix4fv(assets.h_uProjMatrix, 1, GL_FALSE, glm::value_ptr(
                                                                         Projection));
}

void GameState::draw() {
    
//    framebuffer->unbindTexture();
//    framebuffer->bind();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(assets.ShadeProg);
    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    
    
    
    setView();
    setPerspectiveMat();
    
    groundPlane->draw(assets);
    bed->draw(assets);
    
    for(int i = 0; i < clocks.size(); i++){
        clocks[i].draw(assets);
    }
    
    glDisableVertexAttribArray(assets.h_aPosition);
    glDisableVertexAttribArray(assets.h_aNormal);
    
//    framebuffer->unbind();
    
    
    
//    glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
//    
//    // Clear the screen
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glUseProgram(assets.FrameBufferProg);
//    framebuffer->bindTexture(assets.texID);
//    
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
//    glVertexAttribPointer(
//                          assets.quad_vertexPosition_modelspace, // attribute
//                          3,                              // size
//                          GL_FLOAT,                       // type
//                          GL_FALSE,                       // normalized?
//                          0,                              // stride
//                          (void*)0                        // array buffer offset
//                          );
//    
//    // Draw the triangles !
//    glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
//    
//    glDisableVertexAttribArray(0);
//    
//    framebuffer->unbindTexture();
    
    
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    
}
