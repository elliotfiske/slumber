//
//  BillboardActor.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/26/15.
//
//

#include "BillboardActor.h"

/**
 * Construct a new BillboardActor, telling it what texture to display, where to
 *  show it, and how to rotate (only along the Y-axis)
 *
 * Takes in an actor "master billboard" and copies the location of the plane w
 *  sent to the GPU earlier
 */
BillboardActor::BillboardActor(vec3 center_, float scale, float angle, Actor *masterBillboard): Actor(center_) {
    posID[0]    = masterBillboard->posID[0];
    norID[0]    = masterBillboard->norID[0];
    indID[0]    = masterBillboard->indID[0];
    uvID[0]     = masterBillboard->uvID[0];
    numVerts[0] = masterBillboard->numVerts[0];
    numShapes = 1;
    
    direction.y = angle;
}