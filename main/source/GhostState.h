//
//  GhostState.h
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#ifndef __slumber__GhostState__
#define __slumber__GhostState__

#include <stdio.h>
#include "gamestate.hpp"

class GhostState : public GameState {
public:
    GhostState(GLFWwindow *window);
    
protected:
    void checkCollisions();
    void renderScene();
    
    void update();

private:
    std::vector<BoundingBox *> collisions;
};

#endif /* defined(__slumber__GhostState__) */
