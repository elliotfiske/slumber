#include "ViewFrustum.hpp"

ViewFrustum::ViewFrustum() {}

ViewFrustum::~ViewFrustum() {}

void ViewFrustum::extractPlanes(glm::mat4 comboMatrix, bool normalize){
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
    
    
   // Normalize the plane equations, if requested
   if (normalize == true){
      planes[0].makeNormal();
      planes[1].makeNormal();
      planes[2].makeNormal();
      planes[3].makeNormal();
      planes[4].makeNormal();
      planes[5].makeNormal();
   }
}

int ViewFrustum::sphereIsInside(glm::vec3 point, int radius){
   float distance;
   int result = INSIDE;
    
    float leftDistance = leftPlane.distance(point);
    float rightDistance = rightPlane.distance(point);
    
    float nearDistance = nearPlane.distance(point);
    float farDistance = farPlane.distance(point);
    
    float topDistance = topPlane.distance(point);
    float bottomDistance = bottomPlane.distance(point);

//    printf("left dist: %f right dist: %f near: %f far: %f bottom: %f top %f\n", leftDistance, rightDistance, nearDistance, farDistance, bottomDistance, topDistance);
    
   for(int i=0; i < 6; i++) {
       
      distance = planes[i].distance(point);
      if (distance < -radius){
//         return OUTSIDE;
      }
      else if (distance < radius){
//         result = INTERSECT;
      }
   }
//   return result;
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
