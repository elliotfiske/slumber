#include <vector>
#include "actor.hpp"


class GameState{
public: 
   std::vector<Actor> actors;
   
   void update();
   void draw();
};
