#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "Light.h"
#include "shaders/Framebuffer.h"
#include "Texture.h"
#include "ViewFrustum.hpp"
#include "collectible.h"
#include "HUDElement.h"

#ifndef GameState_h
#define GameState_h


class GameState {
public:
    GameState(GLFWwindow *window, bool isGhost);
    
    bool isGhost;
    
    std::vector<Actor> actors;
    Actor *room, *bed, *clock, *enemy, *tv, *door, *fan;
    Actor *lamp;
    Actor *nightstand, *doll, *mirror, *chair;
    Collectible *collectible;
    
    Light *light;

    Framebuffer *framebuffer;
    Framebuffer *shadowfbo;
    Framebuffer *reflectbuffer;
    
    virtual void update();
    void draw();
    virtual void drawHUD();
    
    float playerHealth;
    
    GLuint quad_vertexbuffer, quad_vertexbuffer_mirror;
    
    mat4 perspectiveMat;
    mat4 viewMat;
    mat4 highlightVPMat;
    mat4 mirrorViewMat;
    
    ViewFrustum *vf;
    
    GLuint antialiasTexture, intermediateFBO;
    
    /* Swapping game states */
    bool shouldSwitch;
    virtual GameState* newState();
    
    HUDElement *ghostWins, *playerWins;
    bool ghost_beat_player, player_beat_ghost;
    
protected:
    Camera *camera;
    Camera *mirrorCamera;
    GLFWwindow *window;
    
    double prevTime, elapsedTime;
    
    void updatePerspectiveMat();
    void updateViewMat();
	void updateHighlightMat();
    virtual void renderScene(bool isMirror) {}
    void renderShadowBuffer();
    void renderFrameBuffer();
    void renderReflectBuffer();
    
    virtual void checkCollisions() {}
    
    void initAssets();
    
    void viewFrustumCulling(Actor curActor);
    void tellClientWhereGhostIs();
	void updateDoorSwing();
	void updateDoorSlam();
	void lightFlicker();
	void lightExplode();

	void spinFan();

	double flickerDuration;
	double flickerDirection;
	float attenFactor;

	double explodeDuration;
	bool lampExplode;

	bool doorToggle;
	bool doorSlam;
	int doorDirection;

	bool shakeCamera;

    float clockShakeDuration;
    float tvStaticDuration;
    float dollGlowDuration;
    float dollMoveDuration;
	float fanSpinDuration;

	float FOV;
    
    float playerFOV;
};

#endif
