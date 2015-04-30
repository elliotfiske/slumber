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
	
	void step(double dt);
	bool detectIntersect(Actor target, bool oc);
	void draw(Assets assets);
	
	GLuint posID, norID, indID;
	int numVerts;

	void die();
   int timeToDeath;

private:
   void setModel(Assets assets);
   int collisionCooldown;
};


