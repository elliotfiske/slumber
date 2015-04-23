#include "glm/glm.hpp"
#include "assets.hpp"
using namespace glm;



class Actor {    
public:
	Actor(vec3 center_, vec3 direction_, float velocityScale, float radius);
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
	void draw(Assets assets);
	
	GLuint posID, norID, indID;
	int numVerts;
   int jiggly = 0;

private:
   void setModel(Assets assets);
   void setMaterial(Assets assets);
   int collisionCooldown;
   
};


