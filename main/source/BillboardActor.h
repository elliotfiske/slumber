//
//  BillboardActor.h
//  slumber
//
//  Created by Elliot Fiske on 5/26/15.
//
//

#ifndef __slumber__BillboardActor__
#define __slumber__BillboardActor__

#include <stdio.h>
#include "actor.hpp"

class BillboardActor : public Actor {
public:
    BillboardActor(vec3 center_, float scale, float angle, Actor *masterBillboard);
    
    void draw(Light *light, bool tv = false);
    void setModel();
  
    float angle;
};

#endif /* defined(__slumber__BillboardActor__) */
