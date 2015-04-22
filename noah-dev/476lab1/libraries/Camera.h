//
// sueda
// November, 2014
//

#pragma  once
#ifndef __Camera__
#define __Camera__

#define GLM_FORCE_RADIANS
#include <Eigen/Dense>
#include <Eigen/Geometry>

class MatrixStack;
class CollisionBox;

class Camera
{
public:
	
	Camera();
	virtual ~Camera();
	void setWindowSize(float w, float h);
	void update(float yaw, float pitch, Eigen::Vector3f pos);
	void applyProjectionMatrix(MatrixStack *P) const;
	void applyViewMatrix(MatrixStack *MV) const;
	float getYaw() const { return yawRotation; }
	float getPitch() const { return pitchRotation; }
	float getAspect() const { return aspect; }
	Eigen::Vector3f getPosition() const { return position; }
	
private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	float yawRotation;
	float pitchRotation;
	Eigen::Vector2f mousePrev;
	Eigen::Vector3f position;
};

#endif
