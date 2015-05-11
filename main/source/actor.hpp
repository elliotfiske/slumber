#ifndef actor_h
#define actor_h

#include "glm/glm.hpp"
#include "GLSL.h"
#include "Light.h"

using namespace glm;
using namespace std;

class Actor {
public:
    Actor(vec3 center_);
    vec3 center;
    vec3 direction;
    float velocityScalar;
    float boundSphereRad;
    
    /** Material properties */
    vec3 diffuseColor;
    vec3 ambientColor;
    vec3 specularColor;
    float shininess;
    
    material_t currMaterial;
    
    void step(double dt);
    bool detectIntersect(Actor target, bool oc);
    void draw(Light *light);
    void drawShadows(Light *light);
    
    GLuint posID, norID, indID, uvID;
    int numVerts;
    
private:
    void setModel();
    void setMaterial();
    void setLightMVP(Light *light, bool isShadowShader);
};

#endif
