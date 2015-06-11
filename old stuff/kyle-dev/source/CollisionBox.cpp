#include "../libraries/CollisionBox.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

using namespace std;

CollisionBox::CollisionBox(Eigen::Vector3f min, Eigen::Vector3f max) :
_min(min),
_max(max)
{
}

CollisionBox::~CollisionBox()
{
}

void CollisionBox::setBounds(Eigen::Vector3f min, Eigen::Vector3f max) {
	_min(0) = min(0);
	_max(0) = max(0);
	_min(1) = min(1);
	_max(1) = max(1);
	_min(2) = min(2);
	_max(2) = max(2);
}

bool CollisionBox::isXZCollision(const CollisionBox &box) {
   return !(_max(0) < box.getMinX() || box.getMaxX() < _min(0) || _max(2) < box.getMinZ() || box.getMaxZ() < _min(2));
}

bool CollisionBox::isXCollision(const CollisionBox &box) {
   return !(_max(0) < box.getMinX() || box.getMaxX() < _min(0));
}

bool CollisionBox::isZCollision(const CollisionBox &box) {
   return !(_max(2) < box.getMinZ() || box.getMaxZ() < _min(2));
}

bool CollisionBox::isCollision(const CollisionBox &box) {
   return isXZCollision(box) && !(_max(1) < box.getMinY() || box.getMaxY() < _min(1));
}

void CollisionBox::draw() {
	float currentColor[4];
	glGetFloatv(GL_CURRENT_COLOR, currentColor);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);

	glVertex3fv(_min.data());
	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _min(2)).data());

	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _min(2)).data());
	glVertex3fv(Eigen::Vector3f(_max(0), _max(1), _min(2)).data());

	glVertex3fv(Eigen::Vector3f(_max(0), _max(1), _min(2)).data());
	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _min(2)).data());

	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _min(2)).data());
	glVertex3fv(_min.data());

	glVertex3fv(Eigen::Vector3f(_min(0), _min(1), _max(2)).data());
	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _max(2)).data());

	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _max(2)).data());
	glVertex3fv(_max.data());

	glVertex3fv(Eigen::Vector3f(_max(0), _max(1), _max(2)).data());
	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _max(2)).data());

	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _max(2)).data());
	glVertex3fv(Eigen::Vector3f(_min(0), _min(1), _max(2)).data());

	glVertex3fv(_min.data());
	glVertex3fv(Eigen::Vector3f(_min(0), _min(1), _max(2)).data());

	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _min(2)).data());
	glVertex3fv(Eigen::Vector3f(_max(0), _min(1), _max(2)).data());

	glVertex3fv(Eigen::Vector3f(_max(0), _max(1), _min(2)).data());
	glVertex3fv(_max.data());

	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _min(2)).data());
	glVertex3fv(Eigen::Vector3f(_min(0), _max(1), _max(2)).data());

	glEnd();
	glColor4f(currentColor[0], currentColor[1], currentColor[2], currentColor[3]);
}
