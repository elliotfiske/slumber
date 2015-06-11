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
    
    void renderScene(bool isMirror);
    void update();
    
    BillboardActor *title, *play, *playGhost;
    
    GameState* newState();
};

#endif /* defined(__slumber__titlestate__) */
