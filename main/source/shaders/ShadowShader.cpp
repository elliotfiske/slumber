//
//  ShadowShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/21/15.
//
//

#include "ShadowShader.h"
#include "../glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

/**
 * Initialize and link a vertex and fragment shadow shader
 *  from the specified file names.
 */
ShadowShader::ShadowShader(string vertexShaderFile, std::string fragmentShaderFile) {
    shadow_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(shadow_ProgramID, "vertPos");
    
    // Make handles to uniforms
    MVP_UniformID = GLSL::getUniformLocation(shadow_ProgramID, "MVP");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << endl;
    }
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShadowShader::setIndexArray(GLuint arrayID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayID);
}
