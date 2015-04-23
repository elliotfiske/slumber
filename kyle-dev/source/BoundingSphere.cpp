#include "BoundingSphere.h"

Vector3f farthestFrom(Vector3f v, vector<Vector3f> arr) {
	Vector3f tmp = arr[0];
	Vector3f diff = arr[0] - v;
	float min_dist = diff.dot(diff);

	// compare squared distances for efficiency
	for (unsigned int i = 1; i < arr.size(); i++) {
		diff = arr[i] - v;
		float dist = diff.dot(diff);
		if (dist < min_dist) {
			min_dist = dist;
			tmp = arr[i];
		}
	}

	return tmp;
}

BoundingSphere::BoundingSphere(Eigen::Vector3f center, float radius) :
	_center(center),
	_radius(radius) {
}

BoundingSphere::~BoundingSphere() {
}

bool BoundingSphere::contains(Eigen::Vector3f point) {
	Eigen::Vector3f distance = point - _center;
	return distance.norm() <= _radius;
}

BoundingSphere BoundingSphere::generate(vector<float> positions) {
	vector<Vector3f> vertices;
	// convert float positions into vector3f's for simplicity
	for (unsigned int i = 0; i < positions.size(); i += 3) {
		Vector3f vertex(positions[i], positions[i + 1], positions[i + 2]);
		vertices.push_back(vertex);
	}

	Vector3f a = farthestFrom(vertices[i]);
	Vector3f b = farthestFrom(a);
	Vector3f center = (a + b) / 2.0f;
	BoundingSphere bounds(center, (center - a).norm());

	// ensure that every vertex is enclosed in the sphere
	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vector3f v = vertices[i];
		if (!bounds.contains(v)) {
			Vector3f tmp = bounds.getCenter() - v;
			tmp.normalize();
			Vector3f endPoint = tmp * bounds.getRadius() + bounds.getCenter();
			Vector3f newCenter = (endPoint + v) / 2.0f;
			BoundingSphere newBounds(newCenter, (endPoint - newCenter).norm());
			bounds = newBounds;
		}
	}

	return bounds;
}
