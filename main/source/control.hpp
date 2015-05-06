#include "GLSL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.hpp"

void updateCamDirection(Camera *camera);
void updateControl(GLFWwindow *window);
void setupCallbacks(GLFWwindow *window);

float getForwardVelocity();
float getStrafeVelocity();

