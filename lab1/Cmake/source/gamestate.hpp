#include <vector>
#include "camera.hpp"
#include "control.hpp"
#include "GLSL.h"
#include "assets.hpp"

class GameState {
   public: 
      GameState(GLFWwindow *window);
      std::vector<Actor> actors;
      Control control;
      bool completed;
      
      void update();
      void draw();
      
      Assets assets;
      
   private:
      std::vector<Camera> camera;
      GLFWwindow *window;

      void setPerspectiveMat();
};
