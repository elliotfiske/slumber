

#include "ReflectShader.h"
#include "../glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

/**
 * Initialize and link a vertex and fragment reflection shader
 *  from the specified file names.
 */
ReflectShader::ReflectShader(string vertexShaderFile, string fragmentShaderFile): BaseMVPShader(vertexShaderFile, fragmentShaderFile) {
    reflection_ProgramID = linkProgram(vertexShaderFile, fragmentShaderFile); 
    
    // Make handles to attribute data
    position_AttributeID = GLSL::getAttribLocation(reflection_ProgramID, "vertPos");
    
    // Make handles to uniforms
    mirror_UniformID = GLSL::getUniformLocation(reflection_ProgramID, "mirrorViewMat");
    reflection_sampler_ID   = GLSL::getUniformLocation(reflection_ProgramID, "reflection_sampler");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Reflect Shader: " << err << endl;
    }
}


// --------- REFLECTION SHADER SETTERS ------------
void ReflectShader::startUsingShader() {
    glUseProgram(reflection_ProgramID);
}

void ReflectShader::setMirrorViewMatrix(mat4 camViewMat) {
    glUniformMatrix4fv(mirror_UniformID, 1, GL_FALSE, value_ptr(camViewMat));
}

void ReflectShader::setIndexArray(GLuint arrayID) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arrayID);
}
