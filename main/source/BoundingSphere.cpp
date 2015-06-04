#include "BoundingSphere.hpp"
#include "GLIncludes.h"

using namespace std;

BoundingSphere::BoundingSphere() {
}

BoundingSphere::~BoundingSphere() {
}

vec3 farthestFrom(vec3 v, vector<vec3> arr) {
	vec3 tmp = arr[0];
	vec3 diff = arr[0] - v;
	float min_dist = glm::dot(diff, diff);

	// compare squared distances for efficiency
	for (unsigned int i = 1; i < arr.size(); i++) {
		diff = arr[i] - v;
		float dist = glm::dot(diff, diff);
		if (dist < min_dist) {
			min_dist = dist;
			tmp = arr[i];
		}
	}

	return tmp;
}

bool BoundingSphere::contains(vec3 point) {
	vec3 distance = point - _center;
	return glm::length(distance) <= _radius;
}

void BoundingSphere::init(vector<float> positions) {
	vector<vec3> vertices;
	// convert float positions into vec3's for simplicity
	for (unsigned int i = 0; i < positions.size(); i += 3) {
		vec3 vertex(positions[i], positions[i + 1], positions[i + 2]);
		vertices.push_back(vertex);
	}

	vec3 a = farthestFrom(vertices[0], vertices);
	vec3 b = farthestFrom(a, vertices);
	vec3 center = (a + b) / 2.0f;
	BoundingSphere bounds;

	bounds._center = center;
	bounds._radius = glm::length(center - a);

	// ensure that every vertex is enclosed in the sphere
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vec3 v = vertices[i];
		if (!bounds.contains(v)) {
			vec3 tmp = bounds.getCenter() - v;
			tmp = glm::normalize(tmp);
			vec3 endPoint = tmp * bounds.getRadius() + bounds.getCenter();
			bounds._center = (endPoint + v) / 2.0f;
			bounds._radius = glm::length(endPoint - bounds._center);
		}
	}

	this->_center = bounds._center;
	this->_radius = bounds._radius;
}

void BoundingSphere::drawOutline() {
	int discretization = 30;

	/*glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		glm::mat4 M = glm::rotate(2.0f * (float)M_PI * (float) i / (float) discretization, vec3(0, 0, 1));
		vec4 tmp = _center + M * _radius * vec3(1, 0, 0, 0);

		glVertex3f(tmp.x, tmp.y, tmp.z);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		glm::mat4 M = glm::rotate(2.0f * (float)M_PI * (float) i / (float) discretization, vec3(0, 1, 0));
		vec4 tmp = _center + M * _radius * vec3(0, 0, 1);

		glVertex3f(tmp.x, tmp.y, tmp.z);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= discretization; i++) {
		glm::mat4 M = glm::rotate(2.0f * (float)M_PI * (float) i / (float) discretization, vec3(1, 0, 0));
		vec4 tmp = _center + M * _radius * vec3(0, 1, 0);

		glVertex3f(tmp.x, tmp.y, tmp.z);
	}
	glEnd();*/
}
