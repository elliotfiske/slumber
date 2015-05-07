#pragma  once
#ifndef __Light__
#define __Light__

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

class MatrixStack;

class Light
{
public:
	Light();
	virtual ~Light();
	float getAspect() const { return aspect; }
	float getFovy() const { return fovy; }
	float getZnear() const { return znear; }
	float getZfar() const { return zfar; }
	void setAspect(float a) { aspect = a; }
	void setFovy(float f) { fovy = f; }
	void setZnear(float n) { znear = n; }
	void setZfar(float f) { zfar = f; }
	const glm::vec3 &getPosition() const { return position; }
	const glm::vec3 &getTarget() const { return target; }
	const glm::vec3 &getUp() const { return up; }
	void setPosition(const glm::vec3 &p) { position = p; }
	void setTarget(const glm::vec3 &t) { target = t; }
	void setUp(const glm::vec3 &u) { up = u; }
	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
};

#endif
