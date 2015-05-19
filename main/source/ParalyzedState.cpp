//
//  ParalyzedState.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/19/15.
//
//

#include "ParalyzedState.h"

ParalyzedState::ParalyzedState(GLFWwindow *window): GameState(window, false) {
    camera = new Camera(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, -1.0), 0.0, 1.0);
}

void ParalyzedState::checkCollisions() {
    if (vf->gotLight(collectible->center, 5.0)) {
        collectible->collected();
    }
}