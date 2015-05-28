#include "control.hpp"
#include "GLSL.h"
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.hpp"
#include "network.h"
#include <math.h>

float pitch;
float yaw;
float forwardVel;
float strafeVel;

float forwardAccel;
float strafeAccel;

float mouseX, mouseY;
bool startParalyzed = false, startGhost = false, itemAction = false;

#define ACCEL 8.0
#define FRICTION 1.2

void handleMouse(GLFWwindow* window, double currX, double currY) {
    yaw   += (WINDOW_WIDTH  / 2 - currX) / 1000.0;
    pitch += (WINDOW_HEIGHT / 2 - currY) / 1000.0;
    
    if (pitch < glm::radians(-80.0)) {
        pitch = glm::radians(-80.0);
    }
    
    if (pitch > glm::radians(80.0)) {
        pitch = glm::radians(80.0);
    }
    
    if (startParalyzed) {
    	if (yaw < 1.58) {
    		yaw = 1.58;
    	}

    	if (yaw > 4.65) {
    		yaw = 4.65;
    	}
    }

    mouseX = currX;
    mouseY = currY;
}

bool coordsOverPlay(float x, float y) {
    float midDiffX = fabs(mouseX - WINDOW_WIDTH / 2);
    if (midDiffX < 200 && mouseY < 520.2 && mouseY > 407.6) {
        return true;
    }
    return false;
}

bool coordsOverGhost(float x, float y) {
    float midDiffX = fabs(mouseX - WINDOW_WIDTH / 2);
    if (midDiffX < 200 && mouseY < 825.8 && mouseY > 606.5) {
        return true;
    }
    return false;
}

void doClick(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (coordsOverPlay(mouseX, mouseY) && !startParalyzed && !startGhost) {
            startParalyzed = true;
        }
        
        if (coordsOverGhost(mouseX, mouseY) && !startParalyzed && !startGhost) {
            startGhost = true;
        }
    }
}

void handleScroll(GLFWwindow *window, double scrollX, double scrollY) {
    yaw += (scrollX) / 100.0;
    pitch += (scrollY) / 100.0;
    
    if (pitch < glm::radians(-80.0)) {
        pitch = glm::radians(-80.0);
    }
    
    if (pitch > glm::radians(80.0)) {
        pitch = glm::radians(80.0);
    }
    
    if (startParalyzed) {
        if (yaw < 1.58) {
            yaw = 1.58;
        }
        
        if (yaw > 4.65) {
            yaw = 4.65;
        }
    }
}

bool shouldReset = false;

void handleKeypress(GLFWwindow* window, int key, int scanCode, int action,
                    int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(0);
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

	if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            strafeAccel = ACCEL;
        }
        
        if (action == GLFW_RELEASE) {
            strafeAccel = 0;
        }
    }

	if (key == GLFW_KEY_E) {
        if (action == GLFW_PRESS) {
            itemAction = true;
        }
        
        if (action == GLFW_RELEASE) {
            itemAction = false;
        }
    }
    
    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS) {
        int keyVal = key - GLFW_KEY_0;
        char num[5];
        
        sprintf(num, "%d", keyVal);
        
        sendData(num);
    }
    
    if (key == GLFW_KEY_R) {
        shouldReset = true;
    }
    else {
        shouldReset =false;
    }
}

bool shouldWeReset() {
    return shouldReset;
}

void setupCallbacks(GLFWwindow *window) {
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwSetCursorPosCallback(window, handleMouse);
    glfwSetMouseButtonCallback(window, doClick);
    glfwSetScrollCallback(window, handleScroll);
    
    glfwSetKeyCallback(window, handleKeypress);
    
    pitch = 0;
    yaw = M_PI;
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

void updateCamDirection(Camera *camera) {
    glm::vec3
    direction(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
    
    camera->direction = direction;
}

vec2 titleControl() {
    return vec2(mouseX, mouseY);
}

float getForwardVelocity() {
    return forwardVel;
}

float getStrafeVelocity() {
    return strafeVel;
}


float getYaw() {
    return yaw;
}
float getPitch() {
    return pitch;
}

bool shouldStartParalyzed() {
    return startParalyzed;
}

bool shouldStartGhost() {
    return startGhost;
}

bool getItemAction() {
	return itemAction;
}
