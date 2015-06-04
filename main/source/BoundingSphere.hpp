#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;

class BoundingSphere {
public:
	BoundingSphere();
 	~BoundingSphere();
 
 	// use tiny_obj_loader::shape_t.mesh.positions
	void init(std::vector<float> positions);
	vec3 getCenter() { return _center; }
	float getRadius() { return _radius; }
	bool contains(vec3 point);
	void drawOutline();
 
private:
	vec3 _center;
	float _radius;
};

#endif
