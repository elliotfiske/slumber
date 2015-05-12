#ifndef actor_h
#define actor_h

#include "glm/glm.hpp"
#include "GLSL.h"
#include "Light.h"
#include "tiny_obj_loader.h"
#include "Texture.h"

using namespace glm;
using namespace std;

class Actor {
public:
    Actor(vec3 center_);
    vec3 center;
    vec3 direction;
    float velocityScalar;
    float boundSphereRad;
    
    int numShapes;
    
    tinyobj::material_t material[10];
    Texture *texture[10];
    GLuint textureUnit[10];
    
    void step(double dt);
    bool detectIntersect(Actor target, bool oc);
    void draw(Light *light);
    void drawShadows(Light *light);
    
    GLuint posID[10], norID[10], indID[10], uvID[10];
    int numVerts[10];
    
    mat4 modelMat;
    
private:
    void setModel();
    void setMaterial(tinyobj::material_t material);
    void setLightMVP(Light *light, bool isShadowShader);
};

#endif
