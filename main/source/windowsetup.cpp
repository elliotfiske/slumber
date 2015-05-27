#include "windowsetup.hpp"

GLFWwindow* setupWindow(){
    GLFWwindow* window;
    
    if (!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, false);
    
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
    
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glEnable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(.051f, .447f, .60f, 0.0f);
    
    return window;
}
