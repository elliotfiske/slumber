

#include "ReflectShader.h"
#include "../glm/gtc/type_ptr.hpp" //value_ptr

using namespace std;
using namespace glm;

/**
 * Initialize and link a vertex and fragment lighting shader
 *  from the specified file names.
 */
ReflectShader::ReflectShader(string vertexShaderFile, string fragmentShaderFile): BaseMVPShader(vertexShaderFile, fragmentShaderFile) {
    
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
    
    reflection_sampler_ID   = GLSL::getUniformLocation(programID, "reflection_sampler");
    
    // check OpenGL error
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error from Lighting Shader: " << err << endl;
    }
}


