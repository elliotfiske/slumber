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

class TitleState : public GameState {
public:
    TitleState(GLFWwindow *window);
};

#endif /* defined(__slumber__titlestate__) */
