#include <vector>
#include "actor.hpp"
using namespace std;

class GameState{
public: 
   vector<Actor> actors;
   
   void update();
   void draw();
};
