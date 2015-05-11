#include "ViewFrustum.hpp"

ViewFrustum::ViewFrustum() {}

ViewFrustum::~ViewFrustum() {}

void extractPlanes(mat4 comboMatrix, bool normalize){
   // Left clipping plane
   planes[0].setCoeffecients(comboMatrix[3][0] + comboMatrix[0][0],
      comboMatrix[3][1] + comboMatrix[0][1]
      comboMatrix[3][2] + comboMatrix[0][2]
      comboMatrix[3][3] + comboMatrix[0][3]);
   // Right clipping plane
   planes[1].setCoeffecients(comboMatrix[3][0] - comboMatrix[0][0],
      comboMatrix[3][1] - comboMatrix[0][1]
      comboMatrix[3][2] - comboMatrix[0][2]
      comboMatrix[3][3] - comboMatrix[0][3]);
   // Top clipping plane
   planes[2].setCoeffecients(comboMatrix[3][0] - comboMatrix[1][0],
      comboMatrix[3][1] - comboMatrix[1][1]
      comboMatrix[3][2] - comboMatrix[1][2]
      comboMatrix[3][3] - comboMatrix[1][3]);
   // Bottom clipping plane
   planes[3].setCoeffecients(comboMatrix[3][0] + comboMatrix[1][0],
      comboMatrix[3][1] + comboMatrix[1][1]
      comboMatrix[3][2] + comboMatrix[1][2]
      comboMatrix[3][3] + comboMatrix[1][3]);
   // Near clipping plane
   planes[4].setCoeffecients(comboMatrix[3][0] + comboMatrix[2][0],
      comboMatrix[3][1] + comboMatrix[2][1]
      comboMatrix[3][2] + comboMatrix[2][2]
      comboMatrix[3][3] + comboMatrix[2][3]);
   // Far clipping plane
   planes[0].setCoeffecients(comboMatrix[3][0] - comboMatrix[2][0],
      comboMatrix[3][1] - comboMatrix[2][1]
      comboMatrix[3][2] - comboMatrix[2][2]
      comboMatrix[3][3] - comboMatrix[2][3]);
   // Normalize the plane equations, if requested
   if (normalize == true){
      p_planes[0].normalize;
      p_planes[1].normalize;
      p_planes[2].normalize;
      p_planes[3].normalize;
      p_planes[4].normalize;
      p_planes[5].normalize;
   }
}

bool sphereIsInside(vec3 point, int radius){
   float distance;
   int result = INSIDE;

   for(int i=0; i < 6; i++) {
      distance = planes[i].distance(p);
      if (distance < -radius){
         return OUTSIDE;
      }
      else if (distance < radius){
         result = INTERSECT;
      }
   }
   return result;
}
