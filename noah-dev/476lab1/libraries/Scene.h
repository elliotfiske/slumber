#pragma  once
#ifndef __Scene__
#define __Scene__

#include <vector>
#include <Eigen/Dense>
#include "Shape.h"
#include "../libraries/Player.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	void load();
	void draw(MatrixStack &MV, Program *prog);
	void update(const bool *keys, const Eigen::Vector2f &mouse, const Eigen::Vector2f &center, float dt);
	void init();
	Player getPlayer() { return player; }
	int getNumUncollectedObjects();
	
private:

	void spawnObject();
	float timeLastAdded;
	Player player;
	std::vector<Shape> shapes;
	std::vector<WorldObject> objects;
	std::vector<Texture> textures;
	std::vector<int> shapespertex;
};

#endif
