#include <vector>
#include "actor.hpp"
#include "camera.hpp"
#incluse "control.hpp"

class GameState{
public: 
   std::vector<Actor> actors;
   Camera camera;
   Control control;
   bool completed;
   
   void update();
   void draw();
};
