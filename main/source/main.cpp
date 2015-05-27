#include "windowsetup.hpp"
#include "gamestate.hpp"
#include "ParalyzedState.h"
#include "GhostState.h"
#include "titlestate.h"
#include "network.h"
#include <iostream>

#ifdef THREADS
    #include <thread>
#endif

using namespace std;

GameState *gameState;

void doNetworking() {
    
}

int main(int argc, const char* argv[]) {
    GLFWwindow* window;
    
    // Print out executable location 'cuz it often gets put in a weird place
    printf("It's at %s btw\n", *argv);
    
    window = setupWindow();
    
    if (window == NULL) {
        printf("Window was null\n");
        return 1;
    }
    
#ifdef THREADS
    thread *t1;
    
    if (argc > 1) {
        t1 = new thread(doClientNetworking);
    }
    else {
        t1 = new thread(doGhostNetworking);
    }
#endif
    
    bool isGhost = true;
    if (argc > 1) {
        isGhost = false;
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
    
#ifdef THREADS
    t1->join();
#endif
}
