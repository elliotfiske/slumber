//
//  BillboardShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#include "BillboardShader.h"

BillboardShader::BillboardShader(std::string vertexShaderFile, std::string fragmentShaderFile):
BaseMVPShader(vertexShaderFile, fragmentShaderFile) {
    
    // Make handles to attribute data
    uv_AttributeID       = GLSL::getAttribLocation(programID, "aUV");
    
    // handle the texture
    diffuseTexture_UniformID   = GLSL::getUniformLocation(programID, "diffuseTextureSampler");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error from Lighting Shader: " << err << std::endl;
    }
}


void BillboardShader::setUVArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(uv_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(uv_AttributeID, 2, GL_FLOAT, GL_FALSE, 0, 0);
}