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
    BillboardActor *lampText;
    
    void checkCollisions();
    void renderScene();

    void update();
	bool checkBounds(glm::vec3 min, glm::vec3 max);

	glm::vec3 itemUseBounds;
};

#endif /* defined(__slumber__GhostState__) */
