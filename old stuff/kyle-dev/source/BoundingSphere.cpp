#include "../libraries/BoundingSphere.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

BoundingSphere::BoundingSphere() {
}

BoundingSphere::~BoundingSphere() {
}

Eigen::Vector3f farthestFrom(Eigen::Vector3f v, vector<Eigen::Vector3f> arr) {
	Eigen::Vector3f tmp = arr[0];
	Eigen::Vector3f diff = arr[0] - v;
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

bool BoundingSphere::contains(Eigen::Vector3f point) {
	Eigen::Vector3f distance = point - _center;
	return distance.norm() <= _radius;
}

void BoundingSphere::init(vector<float> positions) {
	vector<Eigen::Vector3f> vertices;
	// convert float positions into Eigen::Vector3f's for simplicity
	for (unsigned int i = 0; i < positions.size(); i += 3) {
		Eigen::Vector3f vertex(positions[i], positions[i + 1], positions[i + 2]);
		vertices.push_back(vertex);
	}

	Eigen::Vector3f a = farthestFrom(vertices[0], vertices);
	Eigen::Vector3f b = farthestFrom(a, vertices);
	Eigen::Vector3f center = (a + b) / 2.0f;
	BoundingSphere bounds;

	bounds._center = center;
	bounds._radius = (center - a).norm();

	// ensure that every vertex is enclosed in the sphere
	for (unsigned int i = 0; i < vertices.size(); i++) {
		Eigen::Vector3f v = vertices[i];
		if (!bounds.contains(v)) {
			Eigen::Vector3f tmp = bounds.getCenter() - v;
			tmp.normalize();
			Eigen::Vector3f endPoint = tmp * bounds.getRadius() + bounds.getCenter();
			bounds._center = (endPoint + v) / 2.0f;
			bounds._radius = (endPoint - bounds._center).norm();
		}
	}

	this->_center = bounds._center;
	this->_radius = bounds._radius;
}

void BoundingSphere::drawOutline() {
	int discretization = 30;

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		Eigen::Matrix3f M = Eigen::AngleAxisf(2.0f * (float)M_PI * (float) i / (float) discretization, Eigen::Vector3f::UnitZ()).toRotationMatrix();
		Eigen::Vector3f tmp = _center + M * _radius * Eigen::Vector3f::UnitX();

		glVertex3fv(tmp.data());
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		Eigen::Matrix3f M = Eigen::AngleAxisf(2.0f * (float)M_PI * (float) i / (float) discretization, Eigen::Vector3f::UnitY()).toRotationMatrix();
		Eigen::Vector3f tmp = _center + M * _radius * Eigen::Vector3f::UnitZ();

		glVertex3fv(tmp.data());
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		Eigen::Matrix3f M = Eigen::AngleAxisf(2.0f * (float)M_PI * (float) i / (float) discretization, Eigen::Vector3f::UnitX()).toRotationMatrix();
		Eigen::Vector3f tmp = _center + M * _radius * Eigen::Vector3f::UnitY();

		glVertex3fv(tmp.data());
	}
	glEnd();
}
