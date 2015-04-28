#ifndef assets_h
#define assets_h

#include "GLSL.h"
#include <string>

using namespace std;

class Assets {
public:
    Assets();
    
    
    
    GLuint pos_sphereID, nor_sphereID, ind_sphereID;
    int numVerts_sphere;
    GLuint pos_roomID, nor_roomID, ind_roomID;
    int numVerts_room;
    
    GLuint pos_bedID, nor_bedID, ind_bedID;
    int numVerts_bed;
    
    GLuint pos_clockID, nor_clockID, ind_clockID;
    int numVerts_clock;
    
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
    GLuint ShadeProg;
    
    GLuint quad_programID;
    GLuint quad_vertexPosition_modelspace;
    GLuint texID;
    GLuint timeID;
    
    GLuint FrameBufferProg;
    
    void loadShape(const char* filename, GLuint *posID, GLuint *norID, GLuint *indID, int *numVerts);
    
private:
    void readLevelData(string filename);
    
};

#endif
