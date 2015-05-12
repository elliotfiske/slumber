#include "ViewFrustum.hpp"

ViewFrustum::ViewFrustum() {}

ViewFrustum::~ViewFrustum() {}

void ViewFrustum::extractPlanes(glm::mat4 comboMatrix) {
   // Left clipping plane
//   leftPlane.setCoefficients(comboMatrix[3][0] + comboMatrix[0][0],
//                             comboMatrix[3][1] + comboMatrix[0][1],
//                             comboMatrix[3][2] + comboMatrix[0][2],
//                             comboMatrix[3][3] + comboMatrix[0][3]);
//   // Right clipping plane
//   rightPlane.setCoefficients(comboMatrix[3][0] - comboMatrix[0][0],
//                              comboMatrix[3][1] - comboMatrix[0][1],
//                              comboMatrix[3][2] - comboMatrix[0][2],
//                              comboMatrix[3][3] - comboMatrix[0][3]);
    
    leftPlane.setCoefficients(comboMatrix[0][3] + comboMatrix[0][0],
                              comboMatrix[1][3] + comboMatrix[1][0],
                              comboMatrix[2][3] + comboMatrix[2][0],
                              comboMatrix[3][3] + comboMatrix[3][0]);
    // Right clipping plane
    rightPlane.setCoefficients(comboMatrix[0][3] - comboMatrix[0][0],
                               comboMatrix[1][3] - comboMatrix[1][0],
                               comboMatrix[2][3] - comboMatrix[2][0],
                               comboMatrix[3][3] - comboMatrix[3][0]);

    
   // Top clipping plane
   topPlane.setCoefficients(comboMatrix[0][3] - comboMatrix[0][1],
                            comboMatrix[1][3] - comboMatrix[1][1],
                            comboMatrix[2][3] - comboMatrix[2][1],
                            comboMatrix[3][3] - comboMatrix[3][1]);
   // Bottom clipping plane
   bottomPlane.setCoefficients(comboMatrix[0][3] + comboMatrix[0][1],
                              comboMatrix[1][3] + comboMatrix[1][1],
                              comboMatrix[2][3] + comboMatrix[2][1],
                              comboMatrix[3][3] + comboMatrix[3][1]);
   // Near clipping plane
   nearPlane.setCoefficients(comboMatrix[0][3] + comboMatrix[0][2],
                             comboMatrix[1][3] + comboMatrix[1][2],
                             comboMatrix[2][3] + comboMatrix[2][2],
                             comboMatrix[3][3] + comboMatrix[3][2]);
   // Far clipping plane
   farPlane.setCoefficients(comboMatrix[0][3] - comboMatrix[0][2],
                            comboMatrix[1][3] - comboMatrix[1][2],
                            comboMatrix[2][3] - comboMatrix[2][2],
                            comboMatrix[3][3] - comboMatrix[3][2]);
    
    leftPlane.makeNormal();
    rightPlane.makeNormal();
    topPlane.makeNormal();
    bottomPlane.makeNormal();
    nearPlane.makeNormal();
    farPlane.makeNormal();
}

int ViewFrustum::sphereIsInside(glm::vec3 point, float radius){
    
    float leftDistance = leftPlane.distance(point);
    float rightDistance = rightPlane.distance(point);
    
    float nearDistance = nearPlane.distance(point);
    float farDistance = farPlane.distance(point);
    
    float topDistance = topPlane.distance(point);
    float bottomDistance = bottomPlane.distance(point);\
    
    if (leftDistance < -2.0) {
        return OUTSIDE;
    }

    if (leftDistance < -2.0) {
        return OUTSIDE;
    }
    if (rightDistance < -2.0) {
        return OUTSIDE;
    }
    if (topDistance < -2.0) {
        return OUTSIDE;
    }
    if (bottomDistance < -2.0) {
        return OUTSIDE;
    }
    return INSIDE;
}

bool ViewFrustum::gotLight(glm::vec3 point, float radius){
    
    float leftDistance = leftPlane.distance(point);
    float rightDistance = rightPlane.distance(point);
    
    float sideDiff = fabs(leftDistance - rightDistance);
    
    float nearDistance = nearPlane.distance(point);
    float farDistance = farPlane.distance(point);
    
    float topDistance = topPlane.distance(point);
    float bottomDistance = bottomPlane.distance(point);
    
    float upDownDiff = fabs(topDistance - bottomDistance);
  
    
    if (sideDiff < 10.0 && upDownDiff < 10.0) {
        // play sound
        return true;
    }
    
    return false;
}
