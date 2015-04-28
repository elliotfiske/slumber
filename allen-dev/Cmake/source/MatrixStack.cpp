#include "../libraries/MatrixStack.h"
#include <stdio.h>
#include <vector>


MatrixStack::MatrixStack()
{
	mstack.push(glm::mat4::Identity());
}

MatrixStack::~MatrixStack()
{
}

void MatrixStack::pushMatrix()
{
	const glm::mat4 &top = mstack.top();
	mstack.push(top);
	assert(mstack.size() < 100);
}

void MatrixStack::popMatrix()
{
	assert(!mstack.empty());
	mstack.pop();
	// There should always be one matrix left.
	assert(!mstack.empty());
}

void MatrixStack::loadIdentity()
{
	glm::mat4 &top = mstack.top();
	top = glm::mat4::Identity();
}

void MatrixStack::translate(const glm::vec3 &trans)
{
	glm::mat4 &top = mstack.top();
	glm::mat4 E = glm::mat4::Identity();
	E(0,3) = trans(0);
	E(1,3) = trans(1);
	E(2,3) = trans(2);
	top *= E;
}

void MatrixStack::scale(const glm::vec3 &scale)
{
	glm::mat4 &top = mstack.top();
	glm::mat4 E = glm::mat4::Identity();
	E(0,0) = scale(0);
	E(1,1) = scale(1);
	E(2,2) = scale(2);
	top *= E;
}

void MatrixStack::scale(float s)
{
	glm::mat4 &top = mstack.top();
	glm::mat4 E = glm::mat4::Identity();
	E(0,0) = s;
	E(1,1) = s;
	E(2,2) = s;
	top *= E;
}

void MatrixStack::rotate(float angle, const glm::vec3 &axis)
{
	glm::mat4 &top = mstack.top();
	glm::mat4 E = glm::mat4::Identity();
	E.block<3,3>(0,0) = Eigen::AngleAxisf(angle, axis).toRotationMatrix();
	top *= E;
}

void MatrixStack::multMatrix(const glm::mat4 &matrix)
{
	glm::mat4 &top = mstack.top();
	top *= matrix;
}

void MatrixStack::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	assert(left != right);
	assert(bottom != top);
	assert(zFar != zNear);
	// Sets the top of the stack
	glm::mat4 &M = mstack.top();
	M = glm::mat4::Zero();
	M(0,0) = 2.0f / (right - left);
	M(1,1) = 2.0f / (top - bottom);
	M(2,2) = -2.0f / (zFar - zNear);
	M(0,3) = - (right + left) / (right - left);
	M(1,3) = - (top + bottom) / (top - bottom);
	M(2,3) = - (zFar + zNear) / (zFar - zNear);
	M(3,3) = 1.0f;
}

void MatrixStack::perspective(float fovy, float aspect, float zNear, float zFar)
{
	assert(fovy != 0.0f);
	assert(aspect != 0.0f);
	assert(zFar != zNear);
	// Sets the top of the stack
	glm::mat4 &M = mstack.top();
	M = glm::mat4::Zero();
	float tanHalfFovy = tan(0.5f * fovy);
	M(0,0) = 1.0f / (aspect * tanHalfFovy);
	M(1,1) = 1.0f / (tanHalfFovy);
	M(2,2) = -(zFar + zNear) / (zFar - zNear);
	M(2,3) = -(2.0f * zFar * zNear) / (zFar - zNear);
	M(3,2) = -1.0f;
}
// TODO
//void MatrixStack::frustum(float l, float r, float b, float t, float zNear, float zFar)
//{
//	glm::mat4 &top = mstack.top();
//	top = glm::frustum(l, r, b, t, zNear, zFar);
//}
//
//void MatrixStack::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
//{
//	glm::mat4 &top = mstack.top();
//	top = glm::lookAt(eye, target, up);
//}

const glm::mat4 &MatrixStack::topMatrix() const
{
	return mstack.top();
}

void MatrixStack::print(const glm::mat4 &mat, const char *name) const
{
	if(name) {
		printf("%s = [\n", name);
	}
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			printf("%- 5.2f ", mat(i,j));
		}
		printf("\n");
	}
	if(name) {
		printf("];");
	}
	printf("\n");
}

void MatrixStack::print(const char *name) const
{
	print(mstack.top(), name);
}

void MatrixStack::printStack() const
{
	// Copy everything to a non-const stack
	std::stack<glm::mat4> tempStack = mstack;
	while(!tempStack.empty()) {
		glm::mat4 &top = tempStack.top();
		print(top);
		tempStack.pop();
	}
}
