//
//  ShadowShader.h
//  slumber
//
//  Created by Elliot Fiske on 5/21/15.
//
//

#ifndef __slumber__ShadowShader__
#define __slumber__ShadowShader__

#include <stdio.h>
#include "shader.h"

class ShadowShader {
public:
    ShadowShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    void startUsingShader();
    void setPositionArray(GLuint arrayID);
    void setIndexArray(GLuint arrayID);
    void setMVPmatrix(glm::mat4 MVPmatrix);
    
    // Clean-up
    void disableAttribArrays();
    
    GLuint shadow_ProgramID;
    
    GLuint position_AttributeID;
    GLuint MVP_UniformID;
};


#endif /* defined(__slumber__ShadowShader__) */
