#include <vector>
#include "camera.hpp"
#include "GLSL.h"
#include "assets.hpp"


#include "Framebuffer.h"


#define XMAX 19.0
#define ZMAX 19.0

class GameState {
public:
    GameState(GLFWwindow *window);
    std::vector<Actor> actors;
    std::vector<Actor> clocks;
    Actor *groundPlane, *bed, *clock;
    
    Framebuffer *framebuffer;
    
    bool completed;
    double prevTime;
    int numCurSpheres;
    int numSpheresHit;
    
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
    
    double timeToNextSphere;
    
    void spawnSphere();
    void checkCollisions();
    
    void initAssets();
};
