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

void HUDElement::drawElement(bool stretchy) {
    glDisable(GL_DEPTH_TEST);
    
    if (stretchy) {
        CurrAssets->hudShader->setImageSize(0, 0);
    }
    else {
        CurrAssets->hudShader->setImageSize(texture->width, texture->height);
    }
    
    GLuint uvID = CurrAssets->masterBillboard->uvID[0];
    GLuint posID = CurrAssets->masterBillboard->posID[0];
    GLuint norID = CurrAssets->masterBillboard->norID[0];
    GLuint indID = CurrAssets->masterBillboard->indID[0];
    
    CurrAssets->hudShader->setPositionArray(posID);
    CurrAssets->hudShader->setIndexArray(indID);
    CurrAssets->hudShader->setUVArray(uvID);
    
    CurrAssets->hudShader->setModelMatrix(mat4(1.0f));
    CurrAssets->hudShader->setViewMatrix(mat4(1.0f));
    CurrAssets->hudShader->setProjectionMatrix(mat4(1.0f));
    
    texture->bind(CurrAssets->hudShader->diffuseTexture_UniformID, 0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CurrAssets->masterBillboard->indID[0]);
    glDrawElements(GL_TRIANGLES, CurrAssets->masterBillboard->numVerts[0], GL_UNSIGNED_INT, (void*) 0);
    
    texture->unbind(0);
}