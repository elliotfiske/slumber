#ifndef __slumber__SimpleShader__
#define __slumber__SimpleShader__

#include <stdio.h>
#include "shader.h"

class SimpleShader {
public:
    SimpleShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    void startUsingShader();
    void setPositionArray(GLuint arrayID);
    void setIndexArray(GLuint arrayID);
    void setMVPmatrix(glm::mat4 MVPmatrix);
    void setColorVector(glm::vec4 color);
    
    // Clean-up
    void disableAttribArrays();
    
    GLuint Simple_ProgramID;
    
    GLuint position_AttributeID;
    GLuint MVP_UniformID;
    GLuint Color_UniformID;
};


#endif
