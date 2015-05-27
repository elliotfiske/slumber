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
    printf("It's at %s btw\n", *argv);
    
    window = setupWindow();
    
    if (window == NULL) {
        printf("Window was null\n");
        return 1;
    }
    
    gameState = new TitleState(window);
    
    while(window) {
        if (gameState == NULL) {
            printf("u wot m8\n");
        }
        
        gameState->update();
        gameState->draw();
        
        if (gameState->shouldSwitch) {
            gameState = gameState->newState();
        }
    }
}
