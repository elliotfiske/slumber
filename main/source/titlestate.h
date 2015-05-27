//
//  titlestate.h
//  slumber
//
//  Created by Elliot Fiske on 5/15/15.
//
//

#ifndef __slumber__titlestate__
#define __slumber__titlestate__

#include <stdio.h>
#include "gamestate.hpp"
#include "BillboardActor.h"

class TitleState : public GameState {
public:
    TitleState(GLFWwindow *window);
    
    void renderScene();
    void update();
    
    Collectible *button1, *button2;
    BillboardActor *title;
    
    GameState* newState();
};

#endif /* defined(__slumber__titlestate__) */
