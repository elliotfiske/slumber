#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "Light.h"
#include "shaders/Framebuffer.h"
#include "Texture.h"
#include "ViewFrustum.hpp"
#include "collectible.h"

#ifndef GameState_h
#define GameState_h

class GameState {
public:
    GameState(GLFWwindow *window, bool isGhost);
    
    bool isGhost;
    
    std::vector<Actor> actors;
    Actor *room, *bed, *clock, *enemy;
    Actor *lamp;
    Collectible *collectible;
    
    Light *light;

    Framebuffer *framebuffer;
    Framebuffer *shadowfbo;
    
    virtual void update();
    void draw();
    
    GLuint quad_vertexbuffer;
    
    mat4 perspectiveMat;
    mat4 viewMat;
    
    ViewFrustum *vf;
    
protected:
    Camera *camera;
    GLFWwindow *window;
    
    double prevTime, elapsedTime;
    
    void updatePerspectiveMat();
    void updateViewMat();
    virtual void renderScene() {}
    void renderShadowBuffer();
    void renderFrameBuffer();
    
    virtual void checkCollisions() {}
    
    void initAssets();
    
    void viewFrustumCulling(Actor curActor);
    void tellClientWhereGhostIs();
};

#endif
