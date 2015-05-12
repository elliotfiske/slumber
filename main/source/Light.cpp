#include "Light.h"
#include <iostream>
#include "GLSL.h"
#include "glm/gtc/matrix_transform.hpp"

Light::Light() :
	aspect(1.0f),
	fovy(90.0f),
	znear(0.1f),
	zfar(200.0f),
	position(-15.0f, 11.0f, -85.0f),
	target(0.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f)
{
}

Light::~Light()
{
	
}

glm::mat4 Light::getProjectionMatrix() const
{
    glm::mat4 projection = glm::perspective(fovy, aspect, znear, zfar);
    return projection;
}

glm::mat4 Light::getViewMatrix() const
{
    glm::mat4 cam = glm::lookAt(position, target, up);
    return cam;
}
