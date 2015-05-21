//
//  ParalyzedState.h
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#ifndef __slumber__ParalyzedState__
#define __slumber__ParalyzedState__

#include <stdio.h>
#include "gamestate.hpp"

class ParalyzedState : public GameState {
public:
    ParalyzedState(GLFWwindow *window);
    void update();
    
protected:
    void checkCollisions();
    void renderScene();
};

#endif /* defined(__slumber__ParalyzedState__) */
