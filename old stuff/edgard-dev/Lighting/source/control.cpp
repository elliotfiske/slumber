#include "control.hpp"
#include "GLSL.h"
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.hpp"
#include <math.h>

float pitch;
float yaw;
float forwardVel;
float strafeVel;

float forwardAccel;
float strafeAccel;

#define ACCEL 5.0
#define FRICTION 1.2

void handleMouse(GLFWwindow* window, double currX, double currY) {
   yaw += (WINDOW_WIDTH / 2 - currX) / 1000.0;
   pitch += (WINDOW_HEIGHT / 2 - currY) / 1000.0;

   if (pitch < glm::radians(-80.0)) {
      pitch = glm::radians(-80.0);
   }

   if (pitch > glm::radians(80.0)) {
      pitch = glm::radians(80.0);
   }
}

void handleKeypress(GLFWwindow* window, int key, int scanCode, int action,
      int mods) {
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwDestroyWindow(window);
      glfwTerminate();
   }

   if (key == GLFW_KEY_W) {
      if (action == GLFW_PRESS) {
         forwardAccel = ACCEL;
      }

      if (action == GLFW_RELEASE) {
         forwardAccel = 0;
      }
   }

   if (key == GLFW_KEY_S) {
      if (action == GLFW_PRESS) {
         forwardAccel = -ACCEL;
      }

      if (action == GLFW_RELEASE) {
         forwardAccel = 0;
      }
   }

   if (key == GLFW_KEY_A) {
      if (action == GLFW_PRESS) {
         strafeAccel = -ACCEL;
      }

      if (action == GLFW_RELEASE) {
         strafeAccel = 0;
      }
   }

   if (key == GLFW_KEY_D) {
      if (action == GLFW_PRESS) {
         strafeAccel = ACCEL;
      }

      if (action == GLFW_RELEASE) {
         strafeAccel = 0;
      }
   }
}

void setupCallbacks(GLFWwindow *window) {
   glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
   glfwSetCursorPosCallback(window, handleMouse);

   glfwSetKeyCallback(window, handleKeypress);

   pitch = 0;
   yaw = 0;
   forwardVel = 0;
   strafeVel = 0;
   forwardAccel = 0;
   strafeAccel = 0;
}

void updateControl(GLFWwindow *window) {
   glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

   forwardVel += forwardAccel;
   forwardVel /= FRICTION;

   strafeVel += strafeAccel;
   strafeVel /= FRICTION;
}

void updateCamera(Camera *camera) {
   glm::vec3
         direction(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));

   camera->direction = direction;
}

float getForwardVelocity() {
   return forwardVel;
}

float getStrafeVelocity() {
   return strafeVel;
}

