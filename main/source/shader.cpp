//
//  shader.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/1/15.
//
//

#include "shader.h"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <cassert>

using namespace std;
using namespace glm;

/**
 * Do the actual compiling + linking of the shader
 */
GLuint linkProgram(string vertexShaderFile, string fragmentShaderFile) {
    vertexShaderFile   = "resources/" + vertexShaderFile;
    fragmentShaderFile = "resources/" + fragmentShaderFile;
    
#ifdef XCODE_IS_TERRIBLE
    vertexShaderFile   = "../" + vertexShaderFile;
    fragmentShaderFile = "../" + fragmentShaderFile;
#endif
    
    GLint error_flag;
    GLuint new_ProgramID;

    // Create shader handles
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    
    // Read shader sources
    const char *vshader = GLSL::textFileRead(vertexShaderFile.c_str());
    const char *fshader = GLSL::textFileRead(fragmentShaderFile.c_str());
    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);
    
    // Compile vertex shader
    glCompileShader(VS);
    GLSL::printError();
    glGetShaderiv(VS, GL_COMPILE_STATUS, &error_flag);
    GLSL::printShaderInfoLog(VS);
    if (!error_flag) {
        printf("Error compiling vertex shader %s\n", vertexShaderFile.c_str());
    }
    
    // Compile fragment shader
    glCompileShader(FS);
    GLSL::printError();
    glGetShaderiv(FS, GL_COMPILE_STATUS, &error_flag);
    GLSL::printShaderInfoLog(FS);
    if (!error_flag) {
        printf("Error compiling fragment shader %s\n", fragmentShaderFile.c_str());
    }
    
    // Create the program and link
    new_ProgramID = glCreateProgram();
    glAttachShader(new_ProgramID, VS);
    glAttachShader(new_ProgramID, FS);
    glLinkProgram(new_ProgramID);
    
    GLSL::printError();
    glGetProgramiv(new_ProgramID, GL_LINK_STATUS, &error_flag);
    GLSL::printProgramInfoLog(new_ProgramID);
    if (!error_flag) {
        printf("Error linking shaders %s and %s\n", vertexShaderFile.c_str(), fragmentShaderFile.c_str());
    }
    
    return new_ProgramID;
}

/**
 * Initialize and link a vertex and fragment lighting shader
 *  from the specified file names.
 */
LightingShader::LightingShader(string vertexShaderFile, string fragmentShaderFile) {
    lighting_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(lighting_ProgramID, "aPosition");
    normal_AttributeID   = GLSL::getAttribLocation(lighting_ProgramID, "aNormal");
    uv_AttributeID       = GLSL::getAttribLocation(lighting_ProgramID, "aUV");
    textureToDisplay_ID  = GLSL::getUniformLocation(lighting_ProgramID, "shadowMap");
    
    // Make handles to uniforms
    projectionMatrix_UniformID = GLSL::getUniformLocation(lighting_ProgramID, "uProjMatrix");
    viewMatrix_UniformID       = GLSL::getUniformLocation(lighting_ProgramID, "uViewMatrix");
    modelMatrix_UniformID      = GLSL::getUniformLocation(lighting_ProgramID, "uModelMatrix");
    lightPos_UniformID         = GLSL::getUniformLocation(lighting_ProgramID, "lightPos");
    ambientMaterial_uniformID  = GLSL::getUniformLocation(lighting_ProgramID, "UaColor");
    diffuseMaterial_UniformID  = GLSL::getUniformLocation(lighting_ProgramID, "UdColor");
    specularMaterial_UniformID = GLSL::getUniformLocation(lighting_ProgramID, "UsColor");
    shininess_UniformID        = GLSL::getUniformLocation(lighting_ProgramID, "Ushine");
    lightMVP_UniformID         = GLSL::getUniformLocation(lighting_ProgramID, "lightMVP");
    
    diffuseTexture_UniformID   = GLSL::getUniformLocation(lighting_ProgramID, "diffuseTextureSampler");
    
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * Initialize and link a vertex and fragment FBO shader
 *  from the specified file names.
 */
FBOShader::FBOShader(std::string vertexShaderFile, std::string fragmentShaderFile) {
    fbo_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(fbo_ProgramID, "aPosition");
    
    // Make handles to uniforms
    textureToDisplay_ID = GLSL::getUniformLocation(fbo_ProgramID, "uTex");
    intensity_UniformID = GLSL::getUniformLocation(fbo_ProgramID, "intensity");
    
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * Initialize and link a vertex and fragment shadow shader
 *  from the specified file names.
 */
ShadowShader::ShadowShader(std::string vertexShaderFile, std::string fragmentShaderFile) {
    shadow_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(shadow_ProgramID, "vertPos");
    
    // Make handles to uniforms
    MVP_UniformID = GLSL::getUniformLocation(shadow_ProgramID, "MVP");
    
    assert(glGetError() == GL_NO_ERROR);
}

// --------- SHADOW SHADER SETTERS ------------
void ShadowShader::startUsingShader() {
    glUseProgram(shadow_ProgramID);
}

void ShadowShader::setPositionArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(position_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(position_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void ShadowShader::setMVPmatrix(mat4 MVPmatrix) {
    glUniformMatrix4fv(MVP_UniformID, 1, GL_FALSE, value_ptr(MVPmatrix));
}

void ShadowShader::disableAttribArrays() {
    glDisableVertexAttribArray(position_AttributeID);
}


// --------- LIGHTING SHADER SETTERS -----------
void LightingShader::startUsingShader() {
    glUseProgram(lighting_ProgramID);
}

void LightingShader::setPositionArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(position_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(position_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void LightingShader::setUVArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(uv_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(uv_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void LightingShader::setNormalArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(normal_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(normal_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void LightingShader::setIndexArray(GLuint arrayID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayID);
}

void LightingShader::setProjectionMatrix(mat4 projectionMatrix) {
    glUniformMatrix4fv(projectionMatrix_UniformID, 1, GL_FALSE, value_ptr(projectionMatrix));
}

void LightingShader::setViewMatrix(mat4 viewMatrix) {
    glUniformMatrix4fv(viewMatrix_UniformID, 1, GL_FALSE, value_ptr(viewMatrix));
}

void LightingShader::setModelMatrix(mat4 modelMatrix) {
    glUniformMatrix4fv(modelMatrix_UniformID, 1, GL_FALSE, value_ptr(modelMatrix));
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

void LightingShader::disableAttribArrays() {
    glDisableVertexAttribArray(position_AttributeID);
    glDisableVertexAttribArray(normal_AttributeID);
}


// ----------- FBO SHADER SETTERS ------------
void FBOShader::setIntensity(float intensity) {
    glUniform1f(intensity_UniformID, intensity);
}

void FBOShader::animateIntensity(float min, float max, double currTime, float slowFactor) {
    float newIntensity = sin(currTime * slowFactor) * (max - min) + min;
    setIntensity(newIntensity);
}

