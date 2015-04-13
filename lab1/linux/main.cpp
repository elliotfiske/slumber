#include "gamestate.hpp"
#include "camera.hpp"

int main(int argc, const char* argv[]) {
	Actor actor = *new Actor(vec3(1.0, 2.0, 4.0), vec3(1.0, 0.0, 0.0), 1.0, 1.0);
	Actor actor2 = *new Actor(vec3(1.5, 2.5, 4.5), vec3(1.0, 0.0, 0.0), 1.0, 1.0);
	printf("center is %f, %f, %f\n", actor.center.x, actor.center.y, actor.center.z); 
	printf("direction is %f, %f, %f\n", actor.direction.x, actor.direction.y, actor.direction.z); 
	printf("velocity is %f\n", actor.velocityScalar);
	printf("radius is %f\n", actor.boundSphereRad);
		
	
	printf("center is %f, %f, %f\n", actor.center.x, actor.center.y, actor.center.z);
        printf("intersects with second object: %d\n", actor.detectIntersect(actor2));


	Gamestate gameState = *new Gamestate();
	while(!gameState->completed) {
		gameState.update();
		gameState.draw();
	}

}
