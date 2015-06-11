//
//  HUDShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#include "HUDShader.h"
#include "shader.h"

HUDShader::HUDShader(string vertexShaderFile, string fragmentShaderFile) {
    programID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(programID, "aPosition");
    uv_AttributeID = GLSL::getAttribLocation(programID, "aUV");
    hudTexture_UniformID = GLSL::getAttribLocation(programID, "uTex");
}

void HUDShader::startUsingShader() {
    glUseProgram(programID);
}

void HUDShader::setPositionArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(position_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(position_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void HUDShader::setUVArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(uv_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(uv_AttributeID, 2, GL_FLOAT, GL_FALSE, 0, 0);
}