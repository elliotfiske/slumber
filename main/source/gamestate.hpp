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
    Actor *room, *bed, *clock, *enemy, *tv, *door;
    Actor *lamp;
    Collectible *collectible;
    
    Light *light;

    Framebuffer *framebuffer;
    Framebuffer *shadowfbo;
    
    virtual void update();
    void draw();
    virtual void drawHUD();
    
    GLuint quad_vertexbuffer;
    
    mat4 perspectiveMat;
    mat4 viewMat;
	mat4 highlightVPMat;
    
    ViewFrustum *vf;
    
    GLuint antialiasTexture, intermediateFBO;
    
    /* Swapping game states */
    bool shouldSwitch;
    virtual GameState* newState();
    
protected:
    Camera *camera;
    GLFWwindow *window;
    
    double prevTime, elapsedTime;
    
    void updatePerspectiveMat();
    void updateViewMat();
	void updateHighlightMat();
    virtual void renderScene() {}
    void renderShadowBuffer();
    void renderFrameBuffer();
    
    virtual void checkCollisions() {}
    
    void initAssets();
    
    void viewFrustumCulling(Actor curActor);
    void tellClientWhereGhostIs();
	void updateDoorSwing();

	double flickerDuration;
	double flickerDirection;
	float attenFactor;

	bool doorToggle;
	int doorDirection;
    float clockShakeDuration;
    float tvStaticDuration;
};

#endif
