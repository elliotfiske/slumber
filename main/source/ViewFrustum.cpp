#include "ViewFrustum.hpp"

ViewFrustum::ViewFrustum() {}

ViewFrustum::~ViewFrustum() {}

void ViewFrustum::extractPlanes(glm::mat4 comboMatrix, bool normalize){
   // Left clipping plane
   planes[0].setCoefficients(comboMatrix[3][0] + comboMatrix[0][0],
      comboMatrix[3][1] + comboMatrix[0][1],
      comboMatrix[3][2] + comboMatrix[0][2],
      comboMatrix[3][3] + comboMatrix[0][3]);
   // Right clipping plane
   planes[1].setCoefficients(comboMatrix[3][0] - comboMatrix[0][0],
      comboMatrix[3][1] - comboMatrix[0][1],
      comboMatrix[3][2] - comboMatrix[0][2],
      comboMatrix[3][3] - comboMatrix[0][3]);
   // Top clipping plane
   planes[2].setCoefficients(comboMatrix[3][0] - comboMatrix[1][0],
      comboMatrix[3][1] - comboMatrix[1][1],
      comboMatrix[3][2] - comboMatrix[1][2],
      comboMatrix[3][3] - comboMatrix[1][3]);
   // Bottom clipping plane
   planes[3].setCoefficients(comboMatrix[3][0] + comboMatrix[1][0],
      comboMatrix[3][1] + comboMatrix[1][1],
      comboMatrix[3][2] + comboMatrix[1][2],
      comboMatrix[3][3] + comboMatrix[1][3]);
   // Near clipping plane
   planes[4].setCoefficients(comboMatrix[3][0] + comboMatrix[2][0],
      comboMatrix[3][1] + comboMatrix[2][1],
      comboMatrix[3][2] + comboMatrix[2][2],
      comboMatrix[3][3] + comboMatrix[2][3]);
   // Far clipping plane
   planes[0].setCoefficients(comboMatrix[3][0] - comboMatrix[2][0],
      comboMatrix[3][1] - comboMatrix[2][1],
      comboMatrix[3][2] - comboMatrix[2][2],
      comboMatrix[3][3] - comboMatrix[2][3]);
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

   for(int i=0; i < 6; i++) {
      distance = planes[i].distance(point);
      if (distance < -radius){
         return OUTSIDE;
      }
      else if (distance < radius){
         result = INTERSECT;
      }
   }
   return result;
}
