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
    BillboardActor(vec3 center_, Texture *texture, float scale, float angle);
  
    float angle;
};

#endif /* defined(__slumber__BillboardActor__) */
