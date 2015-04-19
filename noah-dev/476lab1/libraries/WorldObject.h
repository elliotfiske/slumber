#pragma  once
#ifndef __WorldObject__
#define __WorldObject__

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <cmath>
#include "../libraries/GLSL.h"
#include "../libraries/MatrixStack.h"
#include "../libraries/Shape.h"
#include "../libraries/Program.h"
#include "../libraries/CollisionBox.h"

class WorldObject {
public:
	WorldObject(Shape *shape, Eigen::Vector3f scale = Eigen::Vector3f(1.0f, 1.0f, 1.0f), Eigen::Vector3f x = Eigen::Vector3f(0.0f, 0.0f, 0.0f), Eigen::Vector3f v = Eigen::Vector3f(0.0f, 0.0f, 0.0f));
	virtual ~WorldObject();

	void init(void);
	void checkBounds(CollisionBox arena);
	void draw(MatrixStack &MV, Program *prog) const;
	void update(float dt, std::vector<WorldObject> &objects);
	void buildTexMatrix(float, float);
	Eigen::Matrix3f &getTexMatrix() { return texMat; }
	void setShape(Shape *s) { shape = s; }
	Shape* getShape() { return shape; }
	void setTransform(Eigen::Matrix4f t) { transform = t; }
	CollisionBox* getBox(void) { return box; }
	void setVelocity(Eigen::Vector3f v) { velocity = v; }
	void collect();
	bool getIsCollected() { return isCollected; }

private:
	Shape *shape;
	Eigen::Matrix3f texMat;
	Eigen::Vector3f diffuse;
	Eigen::Vector3f scale;
	Eigen::Vector3f position;
	Eigen::Vector3f velocity;
	Eigen::Quaternionf orientation;
	CollisionBox *box;
	Eigen::Matrix4f transform;
	bool isCollected;
	float yaw;
};

#endif
