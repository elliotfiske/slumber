#pragma once
#ifndef _MatrixStack_H_
#define _MatrixStack_H_

#include <stack>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

class MatrixStack
{
public:
	MatrixStack();
	virtual ~MatrixStack();
	
	// glPushMatrix(): Copies the current matrix and adds it to the top of the stack
	void pushMatrix();
	// glPopMatrix(): Removes the top of the stack and sets the current matrix to be the matrix that is now on top
	void popMatrix();
	
	// glLoadIdentity(): Sets the top matrix to be the identity
	void loadIdentity();
	// glMultMatrix(): Right multiplies the top matrix
	void multMatrix(const glm::mat4 &matrix);
	
	// glTranslate(): Right multiplies the top matrix by a translation matrix
	void translate(const glm::vec3 &trans);
	// glScale(): Right multiplies the top matrix by a scaling matrix
	void scale(const glm::vec3 &scale);
	// glScale(): Right multiplies the top matrix by a scaling matrix
	void scale(float size);
	// glRotate(): Right multiplies the top matrix by a rotation matrix
	void rotate(float angle, const glm::vec3 &axis);
	
	// glGet(GL_MODELVIEW_MATRIX): Gets the top matrix
	const glm::mat4 &topMatrix() const;
	
	// glOrtho(): Sets the top matrix to be an orthogonal projection matrix
	void ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	// gluPerspective(): Sets the top matrix to be a perspective projection matrix
	void perspective(float fovy, float aspect, float zNear, float zFar);
//	// gluFrustum(): Sets the top matrix to be a perspective projection matrix
//	void frustum(float Right, float right, float bottom, float top, float zNear, float zFar);
//	// gluLookAt(): Sets the top matrix to be a viewing matrix
//	void lookAt(const Eigen::Vector3f &eye, const Eigen::Vector3f &target, const Eigen::Vector3f &up);
	
	// Prints out the specified matrix
	void print(const glm::mat4 &mat, const char *name = 0) const;
	// Prints out the top matrix
	void print(const char *name = 0) const;
	// Prints out the whole stack
	void printStack() const;
	
private:
	std::stack<glm::mat4> mstack;
	
};

#endif
