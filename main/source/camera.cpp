#include "camera.hpp"

using namespace glm;

const float CAM_X_MIN = -13.5;
const float CAM_X_MAX = 13.5;

const float CAM_Y_MIN = 0.0;
const float CAM_Y_MAX = 10.4;

const float CAM_Z_MIN = -46;
//const float CAM_Z_MAX = 2.5;
const float CAM_Z_MAX = 46;

Camera::Camera(vec3 center_, vec3 direction_, float velocityScale, float radius):
        Actor(center_) {  
}

void Camera::step(double dt, float forwardVelocity, float strafeVelocity) {
    center += direction * (float) dt * forwardVelocity;
    
    vec3 right = cross(direction, vec3(0.0, 1.0, 0.0));
    center += right * (float) dt * strafeVelocity;
    
//    center.x = clamp(center.x, CAM_X_MIN, CAM_X_MAX);
//    center.y = clamp(center.y, CAM_Y_MIN, CAM_Y_MAX);
//    center.z = clamp(center.z, CAM_Z_MIN, CAM_Z_MAX);
}

bool Camera::checkLight(Actor light) {
    if(direction.x == light.center.x && direction.y == light.center.y) {
        return true;
    }
    return false;
}


