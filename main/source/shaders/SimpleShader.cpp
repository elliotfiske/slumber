#include "SimpleShader.h"
#include "../glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

/**
 * Initialize and link a vertex and fragment Simple shader
 *  from the specified file names.
 */
SimpleShader::SimpleShader(string vertexShaderFile, std::string fragmentShaderFile) {
    Simple_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile);
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(Simple_ProgramID, "vertPos");
    
    // Make handles to uniforms
    MVP_UniformID = GLSL::getUniformLocation(Simple_ProgramID, "MVP");
    Color_UniformID = GLSL::getUniformLocation(Simple_ProgramID, "vertColor");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Simple shader: " << err << endl;
    }
}

// --------- Simple SHADER SETTERS ------------
void SimpleShader::startUsingShader() {
    glUseProgram(Simple_ProgramID);
}

void SimpleShader::setPositionArray(GLuint arrayID) {
    GLSL::enableVertexAttribArray(position_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, arrayID);
    glVertexAttribPointer(position_AttributeID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void SimpleShader::setMVPmatrix(mat4 MVPmatrix) {
    glUniformMatrix4fv(MVP_UniformID, 1, GL_FALSE, value_ptr(MVPmatrix));
}

void SimpleShader::setColorVector(vec4 color) {
    glUniform4fv(Color_UniformID, 1, value_ptr(color));
}

void SimpleShader::disableAttribArrays() {
    glDisableVertexAttribArray(position_AttributeID);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SimpleShader::setIndexArray(GLuint arrayID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayID);
}
