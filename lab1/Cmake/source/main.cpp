#include "gamestate.hpp"
#include "camera.hpp"
#include "windowsetup.hpp"

int main(int argc, const char* argv[]) {
	Actor actor = *new Actor(vec3(1.0, 2.0, 4.0), vec3(1.0, 0.0, 0.0), 1.0, 1.0);
	printf("center is %f, %f, %f\n", actor.center.x, actor.center.y, actor.center.z); 
	printf("direction is %f, %f, %f\n", actor.direction.x, actor.direction.y, actor.direction.z); 
	printf("velocity is %f\n", actor.velocityScalar);
	printf("radius is %f\n", actor.boundSphereRad);
	
	actor.step(2.0);
	printf("center is %f, %f, %f\n", actor.center.x, actor.center.y, actor.center.z);
	
	//window = setupWindow();
}
