//
//  LightingShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/21/15.
//
//

#include "LightingShader.h"
#include "../glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

/**
 * Initialize and link a vertex and fragment lighting shader
 *  from the specified file names.
 */
LightingShader::LightingShader(string vertexShaderFile, string fragmentShaderFile): BaseMVPShader(vertexShaderFile, fragmentShaderFile) {
    
    // Make handles to attribute data
    uv_AttributeID       = GLSL::getAttribLocation(programID, "aUV");
    textureToDisplay_ID  = GLSL::getUniformLocation(programID, "shadowMap");
    
    // Make handles to uniforms
    lightPos_UniformID         = GLSL::getUniformLocation(programID, "lightPos");
    ambientMaterial_uniformID  = GLSL::getUniformLocation(programID, "UaColor");
    diffuseMaterial_UniformID  = GLSL::getUniformLocation(programID, "UdColor");
    specularMaterial_UniformID = GLSL::getUniformLocation(programID, "UsColor");
    shininess_UniformID        = GLSL::getUniformLocation(programID, "Ushine");
    lightMVP_UniformID         = GLSL::getUniformLocation(programID, "lightMVP");
    
    diffuseTexture_UniformID   = GLSL::getUniformLocation(programID, "diffuseTextureSampler");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Lighting Shader: " << err << endl;
    }
}



// --------- BASE LIGHTING SHADER SHADER SETTERS -----------

void LightingShader::setUVArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(uv_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(uv_AttributeID, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void LightingShader::setLightPos(vec3 lightPos) {
    glUniform3fv(lightPos_UniformID, 1, value_ptr(lightPos));
}

void LightingShader::setAmbientColor(float color[]) {
    glUniform3f(ambientMaterial_uniformID, color[0], color[1], color[2]);
}

void LightingShader::setDiffuseColor(float color[]) {
    glUniform3f(diffuseMaterial_UniformID, color[0], color[1], color[2]);
}

void LightingShader::setSpecularColor(float color[]) {
    glUniform3f(specularMaterial_UniformID, color[0], color[1], color[2]);
}

void LightingShader::setShininess(float shininess) {
    glUniform1f(shininess_UniformID, shininess);
}

void LightingShader::setLightMVP(mat4 lightMVP) {
    glUniformMatrix4fv(lightMVP_UniformID, 1, GL_FALSE, value_ptr(lightMVP));
}
