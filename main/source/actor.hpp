#ifndef actor_h
#define actor_h

#include "glm/glm.hpp"
#include "GLSL.h"

using namespace glm;
using namespace std;

/**
 * Holds on to the ID for an actor's position, normal and index buffer.
 */
typedef struct ActorInfo {
    GLuint posID, norID, indID;
} ActorInfo;


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
    
    void step(double dt);
    bool detectIntersect(Actor target, bool oc);
    void draw();
    
    GLuint posID, norID, indID;
    int numVerts;
    
private:
    void setModel();
    void setMaterial();
    
};

#endif
