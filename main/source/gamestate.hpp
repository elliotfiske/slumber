#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "Light.h"
#include "Framebuffer.h"
#include "Texture.h"

class GameState {
public:
    GameState(GLFWwindow *window, bool isGhost);
    
    bool isGhost;
    
    std::vector<Actor> actors;
    Actor *room, *bed, *clock, *real_bed;
    Light *light;
    
    Framebuffer *framebuffer;
    Framebuffer *shadowfbo;
    
    Texture *bedWood;
    
    void update();
    void draw();
    
    // TODO: move to assets or something
    GLuint quad_vertexbuffer;
    
private:
    Camera *camera;
    GLFWwindow *window;
    
    double prevTime;
    double currTime;
    
    void setPerspectiveMat();
    void setView();
    void renderScene();
    void renderShadowBuffer();
    void renderFrameBuffer();
    
    void checkCollisions();
    
    void initAssets();
    
    void tellClientWhereGhostIs();
};
