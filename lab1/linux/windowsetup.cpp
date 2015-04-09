#include <stdio.h>
#include "GLSL.h"

GLFWwindow* setupWindow(){
   GLFWwindow* window;

   if (!glfwInit()){
      fprintf(stderr, "Failed to initialize GLFW\n");
      return NULL;
   }
   
   window = glfwCreateWindow( 1024, 768, "SLUMBER", NULL, NULL);
   if(window == NULL){
      fprintf(stderr, "Failed to initialize window\n");
      glfwTerminate();
      return NULL;
   }
   
   return window;
}
