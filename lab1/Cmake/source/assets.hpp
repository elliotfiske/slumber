#include "GLSL.h"
#include <string>


class Assets {
   public:
      Assets();
      GLuint pos_sphereID, nor_sphereID, ind_sphereID;

      bool installShaders(const std::string &vShaderName, const std::string &fShaderName);
      GLuint h_aPosition;
      GLuint h_aNormal;
      GLuint h_uProjMatrix ;
      GLuint h_uViewMatrix ;
      GLuint h_uModelMatrix;
      GLuint h_uLightPos;
      GLuint h_uMatAmb;
      GLuint h_uMatDif;
      GLuint h_uMatSpec;
      GLuint h_uMatShine;
      GLuint h_uisTex;
      GLuint h_aTexCoord;
      GLuint h_uTexUnit;
      GLuint ShadeProg;
};
