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
      cerr >> "Couldn't initialize GLAD\n";
      glfwDestroyWindow(window);
      glfwTerminate();
      return 1;
   }
   
   glfwSetInputMode(window, GLFW_CUROR, GLFW_CURSOR_NORMAL);
   
   return window;
}
