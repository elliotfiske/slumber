//
//  HUDElement.cpp
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#include "HUDElement.h"
#include "assets.hpp"

using namespace std;

HUDElement::HUDElement(string textureName, float centerX_, float centerY_) {
    texture = new Texture();
    texture->setFilename(textureName);
    texture->init(false);
    
    centerX = centerX_;
    centerY = centerY_;
}

void HUDElement::drawElement() {
    glDisable(GL_DEPTH_TEST);
    
    CurrAssets->hudShader->startUsingShader();
    GLuint uvID = CurrAssets->masterBillboard->uvID[0];
    
    CurrAssets->hudShader->setUVArray(uvID);
    
    texture->bind(CurrAssets->hudShader->hudTexture_UniformID, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CurrAssets->masterBillboard->indID[0]);
    glDrawElements(GL_TRIANGLES, CurrAssets->masterBillboard->numVerts[0], GL_UNSIGNED_INT, (void*) 0);
    
    texture->unbind(0);
}