#include <vector>
#include "actor.hpp"


class GameState{
public: 
   GameState();
   std::vector<Actor> actors;
   
   void update();
   void draw();
};
