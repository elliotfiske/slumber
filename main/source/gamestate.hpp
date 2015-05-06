#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"

#include "Framebuffer.h"


class GameState {
public:
    GameState(GLFWwindow *window);
    std::vector<Actor> actors;
    Actor *room, *bed, *clock;
    
    Framebuffer *framebuffer;
    
    double prevTime;
    void update();
    void draw();
    
    // TODO: move to assets or something
    GLuint quad_vertexbuffer;
    
private:
    Camera *camera;
    GLFWwindow *window;
    
    void setPerspectiveMat();
    void setView();
    void renderScene();
    void renderFrameBuffer();
    
    void checkCollisions();
    
    void initAssets();
};
