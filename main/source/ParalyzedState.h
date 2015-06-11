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
    void increaseHealth(int healthValue);
    void lowerHealth(int severity);
    void increaseSensitive();
    void lowerSensitive();
    int getHealth();
    bool getSensitivity();
	void checkZoom();
    
    HUDElement *introText;
    int numTimesPressedSpace;
    
    float timeToShowIntro;
    
    void tellGhostWhereImLooking();
    
    void drawHUD();

protected:
    void checkCollisions();
    bool playerSensitivity;

    sf::Listener listener;
    void checkHurt(Actor *danger, int howMuch);
    void renderScene(bool isMirror);
};

#endif /* defined(__slumber__ParalyzedState__) */
