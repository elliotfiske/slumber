#include "assets.hpp"
#include "BoundingBox.hpp"
#include <array>
#include <cmath>
#include <cstdio>

BoundingBox::BoundingBox() {
}

BoundingBox::~BoundingBox() {
}

void BoundingBox::insert(std::vector<float> points) {
	for (size_t i = 0; i < points.size(); i += 3)
		this->insert(vec3(points[i], points[i + 1], points[i + 2]));
}

void BoundingBox::insert(vec3 point) {
	vec3 max = min + size;

	min.x = std::min(min.x, point.x);
	min.y = std::min(min.y, point.y);
	min.z = std::min(min.z, point.z);

	max.x = std::max(max.x, point.x);
	max.y = std::max(max.y, point.y);
	max.z = std::max(max.z, point.z);

	size = max - min;
}

bool BoundingBox::collides(BoundingBox other) {
	return std::abs(min.x - other.min.x) && (size.x + other.size.x)
	    && std::abs(min.y - other.min.y) && (size.y + other.size.y)
	    && std::abs(min.z - other.min.z) && (size.z + other.size.z);
}

void BoundingBox::draw() {
	vec3 max = min + size;
	GLfloat vertices[] = {
		min.x, min.y, min.z, 1.0,
		min.x, min.y, max.z, 1.0,
		min.x, max.y, min.z, 1.0,
		min.x, max.y, max.z, 1.0,
		max.x, min.y, min.z, 1.0,
		max.x, min.y, max.z, 1.0,
		max.x, max.y, min.z, 1.0,
		max.x, max.y, max.z, 1.0
	};
	GLuint indices[] = { 
		0, 1, 1, 5, 5, 4, 4, 0, 0, 2, 1, 3, 4, 6, 5, 7, 7, 6, 6, 2, 2, 3, 3, 7
	};
	GLuint vertId, indId;

	printf("<%f, %f, %f>\n", min.x, min.y, min.z);

	glGenBuffers(1, &vertId);
	
//	glBindBuffer(GL_ARRAY_BUFFER, vertId);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indId);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indId);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CurrAssets->lightingShader->setPositionArray(vertId);
	CurrAssets->lightingShader->setIndexArray(indId);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (void*) 0);
}
