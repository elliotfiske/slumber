#include "windowsetup.hpp"

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
   
   glfwMakeContextCurrent(window);
   
   // Initialize GLAD
   if (!gladLoadGL()) {
      printf("Couldn't initialize GLAD\n");
      glfwDestroyWindow(window);
      glfwTerminate();
      return NULL;
   }
   
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   
   glEnable(GL_DEPTH_TEST);
   glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
  
   return window;
}
