//
//  shader.h
//  slumber
//
//  Created by Elliot Fiske on 5/1/15.
//
//

#ifndef __slumber__shader__
#define __slumber__shader__

#include <stdio.h>
#include <string>
#include "glm/glm.hpp"
#include "GLSL.h"

class LightingShader {
public:
    LightingShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint lighting_ProgramID;
    GLuint textureToDisplay_ID;
    void startUsingShader();
    
    void setPositionArray(GLuint arrayID);
    void setNormalArray(GLuint arrayID);
    void setIndexArray(GLuint arrayID);
    
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setModelMatrix(glm::mat4 modelMatrix);
    void setViewMatrix(glm::mat4 viewMatrix);
    void setLightPos(glm::vec3 lightPos); // NOTE: doesn't actually work, yet
    void setAmbientColor(glm::vec3 color);
    void setDiffuseColor(glm::vec3 color);
    void setSpecularColor(glm::vec3 color);
    void setShininess(float shininess);
    void setLightMVP(glm::mat4 lightMVP);
    
    // Clean-up
    void disableAttribArrays();
    
private:
    GLuint position_AttributeID;
    GLuint normal_AttributeID;
    
    GLuint projectionMatrix_UniformID;
    GLuint viewMatrix_UniformID;
    GLuint modelMatrix_UniformID;
    GLuint lightPos_UniformID;
    GLuint ambientMaterial_uniformID;
    GLuint diffuseMaterial_UniformID;
    GLuint specularMaterial_UniformID;
    GLuint shininess_UniformID;
    GLuint lightMVP_UniformID;
};

class FBOShader {
public:
    FBOShader();
    FBOShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint fbo_ProgramID;
    
    GLuint position_AttributeID;
    GLuint textureToDisplay_ID;
};

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

#endif /* defined(__slumber__shader__) */
