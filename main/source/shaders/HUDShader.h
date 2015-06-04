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

class HUDShader {
public:
    HUDShader(string vertexShaderFile, string fragmentShaderFile);
    
    GLuint programID;
    GLuint position_AttributeID;
    GLuint uv_AttributeID;
    GLuint hudTexture_UniformID;
    
    void startUsingShader();
    void setPositionArray(GLuint arrayID);
    void setUVArray(GLuint arrayID);
};

#endif /* defined(__slumber__HUDShader__) */
