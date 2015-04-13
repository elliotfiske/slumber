#include <vector>
#include "actor.hpp"
#include "camera.hpp"

class GameState{
public: 
   std::vector<Actor> actors;
   bool completed;
   
   void update();
   void draw();
};
