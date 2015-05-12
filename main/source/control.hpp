#include "GLSL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.hpp"
#include "Light.h"

void updateCamDirection(Camera *camera);
void updateLightPosition(Light *light);
void updateControl(GLFWwindow *window);
void setupCallbacks(GLFWwindow *window);

float getForwardVelocity();
float getStrafeVelocity();

float getYaw();
float getPitch();

