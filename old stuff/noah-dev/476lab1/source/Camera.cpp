//
// sueda
// November, 2014
//

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "../libraries/MatrixStack.h"
#include "../libraries/Camera.h"

using namespace std;

Camera::Camera() :
	aspect(1.0f),
	fovy(30.0f/180.0f*M_PI),
	znear(0.1f),
	zfar(1200.0f),
	yawRotation(0.0f),
	pitchRotation(0.0f),
	mousePrev(200.0f, 200.0f),
	position(0.0f, 0.0f, 0.0f)
{
}

Camera::~Camera()
{
	
}

void Camera::setWindowSize(float w, float h)
{
	aspect = w/h;
}

void Camera::update(float yaw, float pitch, Eigen::Vector3f pos)
{
	yawRotation = yaw;
	pitchRotation = pitch;
	position = pos;
}

void Camera::applyProjectionMatrix(MatrixStack *P) const
{
	P->perspective(fovy, aspect, znear, zfar);
}

void Camera::applyViewMatrix(MatrixStack *MV) const
{
	// Create the translation and rotation matrices
	Eigen::Matrix4f T = Eigen::Matrix4f::Identity();
	T(0,3) = position(0);
	T(1,3) = position(1);
	T(2,3) = position(2);
	Eigen::Matrix4f YR = Eigen::Matrix4f::Identity();
	YR.block<3,3>(0,0) = Eigen::AngleAxisf(yawRotation, Eigen::Vector3f(0.0f, 1.0f, 0.0f)).toRotationMatrix();
	Eigen::Matrix4f PR = Eigen::Matrix4f::Identity();
	PR.block<3,3>(0,0) = Eigen::AngleAxisf(pitchRotation, Eigen::Vector3f(1.0f, 0.0f, 0.0f)).toRotationMatrix();

	// The matrix C is the product of these matrices
	Eigen::Matrix4f C = T * YR * PR; // Also apply rotations here
	// The view matrix is the inverse
	Eigen::Matrix4f V = C.inverse();
	// Add to the matrix stack
	MV->multMatrix(V);
}
