#include "GLSL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.hpp"
#include "Light.h"

void updateCamDirection(Camera *camera);
void updateLightPosition(Light *light);
void updateControl(GLFWwindow *window);
void setupCallbacks(GLFWwindow *window);

glm::vec2 titleControl();

float getForwardVelocity();
float getStrafeVelocity();

float getYaw();
float getPitch();

bool shouldWeReset();

bool coordsOverGhost(float x, float y);
bool coordsOverPlay(float x, float y);
bool shouldStartGhost();
bool shouldStartParalyzed();

bool getItemAction();
bool getItemAltAction();

int numSpaces();
