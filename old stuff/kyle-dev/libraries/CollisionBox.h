#pragma once
#ifndef __Collision__
#define __Collision__

#define GLM_FORCE_RADIANS
#include <Eigen/Dense>
#include <Eigen/Geometry>

class CollisionBox
{
public:
	
	CollisionBox(Eigen::Vector3f min, Eigen::Vector3f max);
	virtual ~CollisionBox();

	void setBounds(Eigen::Vector3f min, Eigen::Vector3f max);
	bool isXZCollision(const CollisionBox &box);
	bool isXCollision(const CollisionBox &box);
   	bool isZCollision(const CollisionBox &box);
	bool isCollision(const CollisionBox &box);
	Eigen::Vector3f getMin() const { return _min; }
	Eigen::Vector3f getMax() const { return _max; }
	float getMinX() const { return _min(0); }
	float getMinY() const { return _min(1); }
	float getMinZ() const { return _min(2); }
	float getMaxX() const { return _max(0); }
	float getMaxY() const { return _max(1); }
	float getMaxZ() const { return _max(2); }

	void init();
	void draw();
	
private:
	Eigen::Vector3f _min;
	Eigen::Vector3f _max;
};

#endif
