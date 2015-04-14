#include <vector>
#include "camera.hpp"
#include "control.hpp"
#include "GLSL.h"
#include "assets.hpp"

class GameState {
   public: 
      GameState(GLFWwindow *window);
      std::vector<Actor> actors;
      Actor *groundPlane;
      Control control;
      bool completed;
      double prevTime;
      
      void update();
      void draw();
      
      Assets assets;
      
   private:
      Camera *camera;
      GLFWwindow *window;

      void setPerspectiveMat();
      void setView(); 
};
