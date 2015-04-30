#include "../libraries/Light.h"
#include "../libraries/MatrixStack.h"
#include <iostream>

Light::Light() :
	aspect(1.0f),
	fovy(90.0f/180.0f*M_PI),
	znear(1.0f),
	zfar(10000.0f),
	position(0.0f, 5.0f, 0.0f),
	target(0.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f)
{
}

Light::~Light()
{
	
}

void Light::applyProjectionMatrix(MatrixStack *P) const
{
	P->perspective(fovy, aspect, znear, zfar);
}

void Light::applyViewMatrix(MatrixStack *MV) const
{
	MV->lookAt(position, target, up);
}
