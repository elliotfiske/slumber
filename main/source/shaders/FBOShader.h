//
//  FBOShader.h
//  slumber
//
//  Created by Elliot Fiske on 5/21/15.
//
//

#ifndef __slumber__FBOShader__
#define __slumber__FBOShader__

#include <stdio.h>
#include "shader.h"

class FBOShader {
public:
    FBOShader();
    FBOShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    
    GLuint fbo_ProgramID;
    
    GLuint intensity_UniformID;
    GLuint position_AttributeID;
    GLuint textureToDisplay_ID;
	GLuint darknessModifier_UniformID;
    
    /** OPTIONAL PARAMETERS */
    GLuint time_UniformID;
    
    void setIntensity(float intensity);
	void setDarknessModifier(float darknessModifier);
    void animateIntensity(float min, float max, double currTime, float slowFactor);
};

#endif /* defined(__slumber__FBOShader__) */
