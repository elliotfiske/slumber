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
	
	void step(float dt);
	bool detectIntersect(Actor target);
	void draw(Assets assets);
private:
   void setModel();
};


