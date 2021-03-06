#include "windowsetup.hpp"
#include "gamestate.hpp"
#include "ParalyzedState.h"
#include "GhostState.h"
#include "titlestate.h"
#include <iostream>

using namespace std;

GameState *gameState;

int main(int argc, const char* argv[]) {
    GLFWwindow* window;
    
    // Print out executable location 'cuz it often gets put in a weird place
    cout << "It's at " << *argv << " btw\n";
    
    bool fullscreen = false;
    if (argc == 2) {
        fullscreen = true;
    }
    
    window = setupWindow(fullscreen);
    
    if (window == NULL) {
        printf("Window was null\n");
        return 1;
    }
    
    gameState = new TitleState(window);
    
    while(window) {
        gameState->update();
        gameState->draw();
        
        if (gameState->shouldSwitch) {
            gameState = gameState->newState();
        }
        
        
    }
}
