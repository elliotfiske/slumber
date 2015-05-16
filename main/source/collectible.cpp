//
//  collectible.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/14/15.
//
//

#include "collectible.h"
#include "actor.hpp"
#include "assets.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

using namespace glm;

Collectible::Collectible(Actor actor): Actor(actor.center) {
    posID[0] = actor.posID[0];
    norID[0] = actor.norID[0];
    indID[0] = actor.indID[0];
    numVerts[0] = actor.numVerts[0];
}

/**
 * Every frame, determine if we should appear.
 */
void Collectible::step(double dt) {
    if (timeToAppear > 0) {
        timeToAppear -= dt;
    }
    else if (!visible) {
        // Appear in one of the locations we've specified
        center = collectibleCoords[positionIndex];
        positionIndex = ++positionIndex % POSSIBLE_COLLECTIBLE_POSITIONS;
        visible = true;
    }
}

void Collectible::setModel() {
    glm::mat4 Trans  = glm::translate(glm::mat4(1.0f), center);
    modelMat = Trans;
    
    CurrAssets->collectibleShader->setModelMatrix(Trans);
}

/**
 * Simpler draw b/c we only have 1 shape and don't care about lighting.
 */
void Collectible::draw(Light *light) {
    if (!visible) {
        return;
    }
    
    setModel();

    CurrAssets->collectibleShader->setPositionArray(posID[0]);
    CurrAssets->collectibleShader->setNormalArray(norID[0]);
    CurrAssets->collectibleShader->setIndexArray(indID[0]);
    
    glDrawElements(GL_TRIANGLES, numVerts[0], GL_UNSIGNED_INT, (void*) 0);
    
    CurrAssets->collectibleShader->disableAttribArrays();
}

/**
 * User picked me up!  Hide myself until later.
 */
void Collectible::collected() {
    if (!visible) {
        return;
    }
    
    visible = false;
    
    timeToAppear = 0.1; // TODO: randomize.  maybe.
}