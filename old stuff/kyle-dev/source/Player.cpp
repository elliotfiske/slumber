#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "../libraries/Player.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Player::Player() :
	numCollided(0),
	yawRotation(0.0f),
	pitchRotation(0.0f),
	mousePrev(200.0f, 200.0f),
	position(0.0f, 6.0f, 0.0f)
{
	box = new CollisionBox(position - Eigen::Vector3f(1.0f, 1.0f, 1.0f), position + Eigen::Vector3f(1.0f, 1.0f, 1.0f));
}

Player::~Player()
{
	
}

void Player::update(const bool *keys, const Eigen::Vector2f &mouse, const Eigen::Vector2f &center, float dt, std::vector<WorldObject> objects, Octree *octree)
{
	std::vector<WorldObject> colliding = octree->checkCollisions(this->box);

	// check collisions against other objects
	for (int i = 0; i < (int)colliding.size(); i++) {
		if (!colliding[i].getIsCollected()) {
			objects[i].collect();
			numCollided++;
		}
	}

	// How much has the mouse moved?
	float dx = mouse(0) - center(0);
	float dy = mouse(1) - center(1);

	yawRotation -= (M_PI / 800) * dx;
	pitchRotation -= (M_PI / 800) * dy;
	// bound pitchRotation to [-90,90]
	pitchRotation = std::max(-(float)M_PI / 2, std::min((float)M_PI / 2, pitchRotation));
	float walkspeed;
	walkspeed = keys[' '] ? 100.0 * dt : 50.0 * dt;

	Eigen::Vector3f newPosition = position;
	// Update camera position and orientation here
   if (keys['w'] && !keys['s']) {
      newPosition(2) -= cos(yawRotation) * walkspeed;
      newPosition(0) -= sin(yawRotation) * walkspeed;
   }
   else if (keys['s'] && !keys['w']) {
      newPosition(2) += cos(yawRotation) * walkspeed;
      newPosition(0) += sin(yawRotation) * walkspeed;
   }         
   if (keys['a'] && !keys['d']) {
      newPosition(2) -= cos(yawRotation + M_PI / 2) * walkspeed;
      newPosition(0) -= sin(yawRotation + M_PI / 2) * walkspeed;
   }
   else if (keys['d'] && !keys['a']){
      newPosition(2) += cos(yawRotation + M_PI / 2) * walkspeed;
      newPosition(0) += sin(yawRotation + M_PI / 2) * walkspeed;
   }

	CollisionBox arena = *objects.at(0).getBox();
//	if (newPosition(0) < arena.getMaxX() && newPosition(0) > arena.getMinX() && 
//		 newPosition(2) < arena.getMaxZ() && newPosition(2) > arena.getMinZ()) {
		position = newPosition;
		box->setBounds(position - Eigen::Vector3f(1.0f, 1.0f, 1.0f), position + Eigen::Vector3f(1.0f, 1.0f, 1.0f));
//	}

	// Save last mouse
	//mousePrev = mouse;
}
