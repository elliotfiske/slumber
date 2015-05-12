#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "GLSL.h"
#include "Camera.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "CollisionBox.h"
#include "WorldObject.h"
#include "Framebuffer.h"
#include "Image.h"
#include "Program.h"
#include "Texture.h"
#include "Scene.h"
#include "Player.h"
#include "HUD.h"
#include "Light.h"
#include "Octree.h"

using namespace std;
