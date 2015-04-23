#include "tiny_obj_loader.h"
#include <vector>

using namespace std;
using namespace Eigen;

class BoundingSphere {
public:
	BoundingSphere(Vector3f center, float radius);
	~BoundingSphere();
	Eigen::Vector3f getCenter() { return _center; }
	float getRadius() { return radius; }
	bool contains(Vector3f point);

	// use tiny_obj_loader::shape_t.mesh.positions
	static BoundingSphere generate(vector<float> positions);

private:
	Eigen::Vector3f _center;
	float radius;
}
