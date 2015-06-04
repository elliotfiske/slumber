//
//  collectible.h
//  slumber
//
//  Created by Elliot Fiske on 5/14/15.
//
//

#ifndef __slumber__collectible__
#define __slumber__collectible__

#include <stdio.h>

#include "glm/gtc/matrix_transform.hpp"
#include "Light.h"
#include "actor.hpp"

#define POSSIBLE_COLLECTIBLE_POSITIONS 5

const glm::vec3 collectibleCoords[POSSIBLE_COLLECTIBLE_POSITIONS] = {
    glm::vec3(40.6, 28.7, -103.52),
    glm::vec3(36.57, 26.8, -39.52),
    glm::vec3(15.98, 9.77, -82.9),
    glm::vec3(45.7, 8, -30.5),
    glm::vec3(21.7, 18.5, 0.5),
};

class Collectible : public Actor {
public:
    Collectible(Actor actor);
    
    void step(double dt);
    void draw(Light *light);
    void setModel();
    
    void collected();
    
    double timeToAppear;
    
    bool red;
    
private:
    int positionIndex;
    bool visible;
};



#endif /* defined(__slumber__collectible__) */
