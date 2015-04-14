#include "tiny_obj_loader.h"
#include "GLSL.h"
#include "assets.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cassert>

using namespace std;

bool Assets::installShaders(const std::string &vShaderName, const std::string &fShaderName)
{
   GLint rc;
   
   // Create shader handles
   GLuint VS = glCreateShader(GL_VERTEX_SHADER);
   GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
   
   // Read shader sources
   const char *vshader = GLSL::textFileRead(vShaderName.c_str());
   const char *fshader = GLSL::textFileRead(fShaderName.c_str());
   glShaderSource(VS, 1, &vshader, NULL);
   glShaderSource(FS, 1, &fshader, NULL);
   
   // Compile vertex shader
   glCompileShader(VS);
   GLSL::printError();
   glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(VS);
   if(!rc) {
      printf("Error compiling vertex shader %s\n", vShaderName.c_str());
      return false;
   }
   
   // Compile fragment shader
   glCompileShader(FS);
   GLSL::printError();
   glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
   GLSL::printShaderInfoLog(FS);
   if(!rc) {
      printf("Error compiling fragment shader %s\n", fShaderName.c_str());
      return false;
   }
   
   // Create the program and link
      ShadeProg = glCreateProgram();
      glAttachShader(ShadeProg, VS);
      glAttachShader(ShadeProg, FS);
      glLinkProgram(ShadeProg);
   
      GLSL::printError();
      glGetProgramiv(ShadeProg, GL_LINK_STATUS, &rc);
      GLSL::printProgramInfoLog(ShadeProg);
      if(!rc) {
         printf("Error linking shaders %s and %s\n", vShaderName.c_str(), fShaderName.c_str());
         return false;
      }

   /* get handles to attribute data */
    h_aPosition = GLSL::getAttribLocation(ShadeProg, "aPosition");
    h_aNormal = GLSL::getAttribLocation(ShadeProg, "aNormal");
    h_uProjMatrix = GLSL::getUniformLocation(ShadeProg, "uProjMatrix");
    h_uViewMatrix = GLSL::getUniformLocation(ShadeProg, "uViewMatrix");
    h_uModelMatrix = GLSL::getUniformLocation(ShadeProg, "uModelMatrix");
    h_uLightPos = GLSL::getUniformLocation(ShadeProg, "uLightPos");
    h_uMatAmb = GLSL::getUniformLocation(ShadeProg, "UaColor");
    h_uMatDif = GLSL::getUniformLocation(ShadeProg, "UdColor");
    h_uMatSpec = GLSL::getUniformLocation(ShadeProg, "UsColor");
    h_uMatShine = GLSL::getUniformLocation(ShadeProg, "Ushine");
   
   assert(glGetError() == GL_NO_ERROR);
   return true;
}

Assets::Assets() {
  printf("Loading assets\n");
  std::vector<tinyobj::shape_t>    shapes;
  std::vector<tinyobj::material_t> materials;
  
  std::string err = tinyobj::LoadObj(shapes, materials, "sphere.obj");
  if(!err.empty()) {
    printf("OBJ error: %s\n", err.c_str());
  }
  

    // Send the position array to the GPU
    const vector<float> &posBuf = shapes[0].mesh.positions;
    glGenBuffers(1, &pos_sphereID);
    glBindBuffer(GL_ARRAY_BUFFER, pos_sphereID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    // Send the normal array to the GPU
    const vector<float> &norBuf = shapes[0].mesh.normals;
    glGenBuffers(1, &nor_sphereID);
    glBindBuffer(GL_ARRAY_BUFFER, nor_sphereID);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    
    // Send the index array to the GPU
    const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
    glGenBuffers(1, &ind_sphereID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_sphereID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    
    numVerts_sphere = shapes[0].mesh.indices.size();
    
    if (!installShaders("vert.glsl", "frag.glsl")) {
      printf("Couldn't load shaders :(\n");
    }
    
}


