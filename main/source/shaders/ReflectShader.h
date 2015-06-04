
#ifndef __slumber__ReflectShader__
#define __slumber__ReflectShader__

#include <stdio.h>
#include "shader.h"

class ReflectShader : public BaseMVPShader {
public:
    ReflectShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint reflection_ProgramID;
    GLuint position_AttributeID;
    GLuint mirror_UniformID;
    GLuint reflection_sampler_ID;

    
    
    void startUsingShader();
    void setPositionArray(GLuint arrayID);
    void setIndexArray(GLuint arrayID);
    void setMirrorViewMatrix(glm::mat4 camViewMat);
       
};

#endif /* defined(__slumber__ReflectShader__) */
