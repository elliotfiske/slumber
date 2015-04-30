#include <cstdlib>
#include <iostream>
#include "../libraries/WorldObject.h"
#include "../libraries/Rand.h"

// reverses velocity if moving outside the arena
void WorldObject::checkBounds(CollisionBox arena) {
	if (box->getMaxX() >= arena.getMaxX() && velocity(0) > 0) {
		velocity(0) = -velocity(0);
	}
	if (box->getMinX() <= arena.getMinX() && velocity(0) < 0) {
		velocity(0) = -velocity(0);
	}
	if (box->getMaxZ() >= arena.getMaxZ() && velocity(2) > 0) {
		velocity(2) = -velocity(2);
	}
	if (box->getMinZ() <= arena.getMinZ() && velocity(2) < 0) {
		velocity(2) = -velocity(2);
	}
}

WorldObject::WorldObject(Shape *mesh, Eigen::Vector3f size, Eigen::Vector3f x, Eigen::Vector3f v) :
	shape(mesh),
	scale(size),
	position(x),
	velocity(v),
	isCollected(false),
	yaw(0.0f)
{
   diffuse = Eigen::Vector3f(randomFloat(0.2f, 0.8f), randomFloat(0.2f, 0.8f), randomFloat(0.2f, 0.8f));
	box = new CollisionBox(x - size / 2, x + size / 2);
}


WorldObject::~WorldObject()
{
	
}

void WorldObject::draw(MatrixStack &MV, Program *prog, Light &light, bool isShadowPass1) const {
	MatrixStack lightP, lightMV; // light matrices
	lightP.pushMatrix();
	light.applyProjectionMatrix(&lightP);
	lightMV.pushMatrix();
	light.applyViewMatrix(&lightMV);
	lightMV.translate(position);
	lightMV.rotate(yaw - (float)M_PI/2, Eigen::Vector3f(0.0f, 1.0f, 0.0f));
	lightMV.scale(scale);

	Eigen::Matrix4f lightMVP = lightP.topMatrix() * lightMV.topMatrix();

	if (isShadowPass1) {
		glUniformMatrix4fv(prog->getUniform("MVP"), 1, GL_FALSE, lightMVP.data());
		shape->draw(prog->getAttribute("vertPos"), -1, -1);
		return;
	}

	MV.pushMatrix();
	MV.translate(position);
	MV.rotate(yaw - (float)M_PI/2, Eigen::Vector3f(0.0f, 1.0f, 0.0f));
	MV.scale(scale);
	glUniformMatrix4fv(prog->getUniform("lightMVP"), 1, GL_FALSE, lightMVP.data());
	glUniform3fv(prog->getUniform("kd"),  1, diffuse.data());
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, MV.topMatrix().data());
	glUniformMatrix3fv(prog->getUniform("T1"), 1, GL_TRUE, texMat.data());	
	shape->draw(prog->getAttribute("vertPos"), prog->getAttribute("vertNor"), prog->getAttribute("vertTex"));
	MV.popMatrix();
	lightMV.popMatrix();
	lightP.popMatrix();
}

void WorldObject::update(float dt, std::vector<WorldObject> &objects) {
	if (isCollected == false) {
		// check collisions against other objects
		for (int i = 1; i < (int)objects.size(); i++) {
			if (!objects.at(i).isCollected && box->isXZCollision(*objects.at(i).getBox())) {
				if (velocity.dot(objects.at(i).position - position) > 0) {
					objects.at(i).velocity = -objects.at(i).velocity;
					velocity = -velocity;
				}
				break;
			}
		}

		// update position and bounding box
		Eigen::Vector3f pos0 = position;
		position += velocity * dt;
		//yaw = atan2(position(2) - pos0(2), position(0) - pos0(0));
		yaw = atan2(pos0(0) - position(0), pos0(2) - position(2));
		box->setBounds(box->getMin() + velocity * dt, box->getMax() + velocity * dt);
		checkBounds(*objects.at(0).getBox());
	}
	else {
		if (std::max(scale(0), scale(2)) > 0.5f) {
			scale(0) /= 1.05f;
			scale(1) /= 1.05f;
			scale(2) /= 1.05f;
			position(1) = scale(1);
		}
	}
}

void WorldObject::buildTexMatrix(float scaleX, float scaleY) {
	texMat = Eigen::Matrix3f::Identity();
	texMat(0,0) = scaleX / 5;
	texMat(1,1) = scaleY / 5;
}

void WorldObject::collect() {
	isCollected = true;
}
