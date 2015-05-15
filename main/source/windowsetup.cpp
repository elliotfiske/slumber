#include "windowsetup.hpp"

GLFWwindow* setupWindow(){
   GLFWwindow* window;

   if (!glfwInit()){
      fprintf(stderr, "Failed to initialize GLFW\n");
      return NULL;
   }
    
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_SAMPLES, 4);
   
   window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "SLUMBER", NULL, NULL);
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
   
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   
   glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
   glClearColor(.051f, .447f, .60f, 0.0f);
  
   return window;
}
