#include <stdio.h>
#include "actor.hpp"

int main(int argc, const char* argv[]) {
	Actor actor = *new Actor(1, 2, 4);
	printf("%f, %f, %f\n", actor.x, actor.y, actor.z); 
}
