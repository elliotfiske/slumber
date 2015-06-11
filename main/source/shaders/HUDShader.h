//
//  HUDShader.h
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#ifndef __slumber__HUDShader__
#define __slumber__HUDShader__

#include <stdio.h>
#include "shader.h"

using namespace std;

class HUDShader : public BaseMVPShader {
public:
    HUDShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    GLuint uv_AttributeID;
    GLuint diffuseTexture_UniformID;
    GLuint percentShown_UniformID;
    GLuint screenSize_UniformID;
    GLuint imageSize_UniformID;
    
    void setUVArray(GLuint arrayID);
    void setPercentShown(float percent);
    void setScreenSize(float width, float height);
    void setImageSize(float width, float height);
};

#endif /* defined(__slumber__HUDShader__) */
