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

class BaseMVPShader {
public:
    BaseMVPShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint programID;
    
    void startUsingShader();
    void setNormalArray(GLuint arrayID);
    void setIndexArray(GLuint arrayID);
    
    void setPositionArray(GLuint arrayID);
    void setProjectionMatrix(glm::mat4 projectionMatrix);
    void setModelMatrix(glm::mat4 modelMatrix);
    void setViewMatrix(glm::mat4 viewMatrix);
    
    // Clean-up
    void disableAttribArrays();

protected:
    GLuint position_AttributeID;
    GLuint normal_AttributeID;
    
    GLuint projectionMatrix_UniformID;
    GLuint viewMatrix_UniformID;
    GLuint modelMatrix_UniformID;
};

class LightingShader : public BaseMVPShader {
public:
    LightingShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint textureToDisplay_ID;
    GLuint uv_AttributeID;
    GLuint diffuseTexture_UniformID;
    
    void setUVArray(GLuint arrayID);
    
    void setLightPos(glm::vec3 lightPos);
    void setAmbientColor(float color[]);
    void setDiffuseColor(float color[]);
    void setSpecularColor(float color[]);
    void setShininess(float shininess);
    void setLightMVP(glm::mat4 lightMVP);
    
private:
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
    
    GLuint intensity_UniformID;
    GLuint position_AttributeID;
    GLuint textureToDisplay_ID;
    
    void setIntensity(float intensity);
    void animateIntensity(float min, float max, double currTime, float slowFactor);
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
