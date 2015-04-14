#include <vector>
#include "camera.hpp"
#include "control.hpp"

class GameState {
   public: 
      GameState();
      std::vector<Actor> actors;
      Control control;
      bool completed;
      
      void update();
      void draw();
      
   private:
      std::vector<Camera> camera;
};
