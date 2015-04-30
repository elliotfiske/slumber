#ifndef _BOUNDING_SPHERE_H_
#define _BOUNDING_SPHERE_H_

#include "tiny_obj_loader.h"
#include <vector>
#include <Eigen/Dense>

using namespace std;

class BoundingSphere {
public:
	BoundingSphere();
	~BoundingSphere();

	// use tiny_obj_loader::shape_t.mesh.positions
	void init(vector<float> positions);
	Eigen::Vector3f getCenter() { return _center; }
	float getRadius() { return _radius; }
	bool contains(Eigen::Vector3f point);
	void drawOutline();

private:
	Eigen::Vector3f _center;
	float _radius;
};

#endif
