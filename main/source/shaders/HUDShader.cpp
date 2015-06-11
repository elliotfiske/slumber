//
//  HUDShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#include "HUDShader.h"
#include "shader.h"

HUDShader::HUDShader(std::string vertexShaderFile, std::string fragmentShaderFile):
BaseMVPShader(vertexShaderFile, fragmentShaderFile) {
    
    // Make handles to attribute data
    uv_AttributeID       = GLSL::getAttribLocation(programID, "aUV");
    
    // handle the texture
    diffuseTexture_UniformID   = GLSL::getUniformLocation(programID, "diffuseTextureSampler");
    percentShown_UniformID   = GLSL::getUniformLocation(programID, "percentShown");
    
    screenSize_UniformID = GLSL::getUniformLocation(programID, "screenSize");
    imageSize_UniformID  = GLSL::getUniformLocation(programID, "imageSize");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error from Lighting Shader: " << err << std::endl;
    }
}


void HUDShader::setUVArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(uv_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(uv_AttributeID, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void HUDShader::setPercentShown(float percent) {
    glUniform1f(percentShown_UniformID, percent);
}

void HUDShader::setScreenSize(float width, float height) {
    glUniform2f(screenSize_UniformID, width, height);
}

void HUDShader::setImageSize(float width, float height) {
    glUniform2f(imageSize_UniformID, width, height);
}
