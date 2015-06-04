//
//  FBOShader.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/21/15.
//
//

#include "FBOShader.h"

using namespace std;

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

    time_UniformID = glGetUniformLocation(fbo_ProgramID, "time");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from FBO shader: " << err << endl;
    }
}

void FBOShader::setIntensity(float intensity) {

    glUniform1f(intensity_UniformID, intensity);
}

void FBOShader::animateIntensity(float min, float max, double currTime, float slowFactor) {
    float newIntensity = sin(currTime * slowFactor) * (max - min) + min*2;
    setIntensity(newIntensity);
}
