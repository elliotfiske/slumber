#ifndef actor_h
#define actor_h

#include "glm/glm.hpp"
#include "GLSL.h"
#include "Light.h"
#include "tiny_obj_loader.h"
#include "Texture.h"
#include "BoundingBox.hpp"

using namespace glm;
using namespace std;

#define NUM_SHAPES 100

class Actor {
public:
    Actor(vec3 center_);
    vec3 center;
    vec3 direction;
    float velocityScalar;
    BoundingBox boxes[NUM_SHAPES];
    
    float scale;
    
    tinyobj::material_t material[NUM_SHAPES];
    Texture *texture[NUM_SHAPES];
    GLuint textureUnit[NUM_SHAPES];
    
    void step(double dt);
    
    bool detectIntersect(Actor target, bool oc);
    void draw(Light *light, bool tv = false);
    void drawShadows(Light *light);
    
    int numShapes;
    GLuint posID[NUM_SHAPES], norID[NUM_SHAPES], indID[NUM_SHAPES], uvID[NUM_SHAPES];
    int numVerts[NUM_SHAPES];
    
    mat4 modelMat;
    
    void setModel();
    void setMaterial(tinyobj::material_t material);
    
    /** Hack: which index, if any, is the TV screen?  We want to apply static to it. */
    int tvScreenIndex;
    
private:
    void setLightMVP(Light *light, bool isShadowShader);
};

#endif
