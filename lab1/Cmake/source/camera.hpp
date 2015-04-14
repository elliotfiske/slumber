
#include "actor.hpp"

class Camera : public Actor {
   public:
      Camera();
      Camera(vec3 center_, vec3 direction_, float velocityScale, float radius);

};
