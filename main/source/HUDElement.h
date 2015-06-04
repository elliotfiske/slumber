//
//  HUDElement.h
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#ifndef __slumber__HUDElement__
#define __slumber__HUDElement__

#include <stdio.h>
#include "Texture.h"
#include <string>

class HUDElement {
public:
    /** NOTE that center coords are done with UV coordinates */
    HUDElement(std::string textureName, float centerX_, float centerY_);
    
    void drawElement();
    
    Texture *texture;
    float percentCutoff;
    
    float centerX, centerY;
};

#endif /* defined(__slumber__HUDElement__) */
