#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

#include "glm/glm.hpp"
#include <vector>

using namespace glm;

class BoundingBox {
public:
	BoundingBox();
	~BoundingBox();
	void insert(std::vector<float> points, mat4 modelMat);
	void insert(vec3 point);
	bool collides(BoundingBox other);
	void draw(mat4 MVP);

	vec3 min, size;
};

#endif
