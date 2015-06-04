//
//  BillboardShader.h
//  slumber
//
//  Created by Elliot Fiske on 6/3/15.
//
//

#ifndef __slumber__BillboardShader__
#define __slumber__BillboardShader__

#include <stdio.h>
#include "shader.h"

class BillboardShader : public BaseMVPShader {
public:
    BillboardShader(std::string vertexShaderFile, std::string fragmentShaderFile);
    GLuint uv_AttributeID;
    GLuint diffuseTexture_UniformID;
	GLuint percentShown_UniformID;
    
    void setUVArray(GLuint arrayID);
	void setPercentShown(float percent);
};

#endif /* defined(__slumber__BillboardShader__) */
