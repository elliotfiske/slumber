#pragma  once
#ifndef __Player__
#define __Player__

#define GLM_FORCE_RADIANS
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <stdio.h>
#include "../libraries/CollisionBox.h"
#include "../libraries/WorldObject.h"

class Player {
public:
	Player();
	virtual ~Player();
	void update(const bool *keys, const Eigen::Vector2f &mouse, const Eigen::Vector2f &center, float dt, std::vector<WorldObject> &objects);
	float getYaw() const { return yawRotation; }
	float getPitch() const { return pitchRotation; }
	Eigen::Vector3f getPosition() const { return position; }
	int getNumCollided() { return numCollided; }
	
private:
	int numCollided;
	float yawRotation;
	float pitchRotation;
	Eigen::Vector2f mousePrev;
	Eigen::Vector3f position;
	CollisionBox *box;
};

#endif
