#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "Light.h"
#include "Framebuffer.h"


class GameState {
public:
    GameState(GLFWwindow *window);
    std::vector<Actor> actors;
    Actor *room, *bed, *clock;
    Light *light;
    
    Framebuffer *framebuffer;
    Framebuffer *shadowfbo;
    
    double prevTime;
    void update();
    void draw();
    
    // TODO: move to assets or something
    GLuint quad_vertexbuffer;
    
    mat4 perspectiveMat;
    mat4 viewMat;
    
private:
    Camera *camera;
    GLFWwindow *window;
    
    void setPerspectiveMat();
    void setView();
    void renderScene();
    void renderShadowBuffer();
    void renderFrameBuffer();
    
    void checkCollisions();
    
    void initAssets();
    
    void viewFrustumCulling(Actor curActor);
};
