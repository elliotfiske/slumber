#include "../libraries/CollisionBox.h"

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
