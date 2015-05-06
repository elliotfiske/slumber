#include "windowsetup.hpp"
#include "gamestate.hpp"
#include <thread>

using namespace std;

GameState *gameState;

void doNetworking() {
    
}

int main(int argc, const char* argv[]) {
    GLFWwindow* window;
    
    // Test this test that
    printf("It's at %s btw\n", *argv);
    
    window = setupWindow();
    
    if (window == NULL) {
        printf("Window was null\n");
        return 1;
    }
    
    thread t1(doNetworking);
    
    gameState = new GameState(window);
    while(window) {
        gameState->update();
        gameState->draw();
    }
    
    t1.join();
}
