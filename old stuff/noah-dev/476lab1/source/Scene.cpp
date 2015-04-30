#include "../libraries/Libraries.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../libraries/Rand.h"
#define INTERVAL 5.0f
#define MAX_OBJECTS 50

using namespace std;

Scene::Scene() :
timeLastAdded(0.0f)
{
}

Scene::~Scene()
{
	
}

void Scene::spawnObject() {
	if ((int)objects.size() >= MAX_OBJECTS)
		return;

	CollisionBox *box = objects.at(0).getBox();
	Eigen::Vector3f s;
	float scale = randomFloat(0.5f, 2.0f);
	s(0) = scale;
	s(1) = scale;
	s(2) = scale;

	Eigen::Vector3f x;
	x(0) = randomFloat(-10.0f, 10.0f);
	x(1) = 1.0f;
	x(2) = randomFloat(-10.0f, 10.0f);

	Eigen::Vector3f v;
	v(0) = rand() < 0.5 ? randomFloat(-100.0f, 50.0f) : randomFloat(50.0f, 100.0f);
	v(1) = 0.0f;
	v(2) = rand() < 0.5 ? randomFloat(-100.0f, 50.0f) : randomFloat(50.0f, 100.0f);

	WorldObject newObj(&shapes.at(1), s, x, v);

	objects.push_back(newObj);
}

void Scene::draw(MatrixStack &MV, Program *prog, Light &light, bool isShadowPass1) {
	for (int i = 0; i < (int)objects.size(); i++) {
		objects.at(i).draw(MV, prog, light, isShadowPass1);
	}
}

void Scene::update(const bool *keys, const Eigen::Vector2f &mouse, const Eigen::Vector2f &center, float dt) {
	timeLastAdded += dt;
	if (timeLastAdded >= INTERVAL) {
		timeLastAdded = 0.0f;
		spawnObject();
	}

	player.update(keys, mouse, center, dt, objects);

	/*for (int i = 1; i < (int)objects.size(); i++) {
		std::vector<WorldObject> objsCopy;
		for (int j = 0; j < (int)objects.size(); j++) {
			if (i != j)
				objsCopy.push_back(objects.at(j));
		}
		objects.at(i).update(dt, objsCopy);
	}*/
}

void Scene::init() {
	for (int i = 0; i < (int)shapes.size(); i++) {
		shapes.at(i).init();
		//textures.at(i).init();
	}
}

// Loads the attachment file (bone weights for each vertex)
void Scene::load() {
	int numShapes = 2;

	shapes.resize(numShapes);
	//textures.resize(numShapes);
	shapespertex.resize(numShapes);

	Shape tempShape;
	tempShape.load("../materials/cube.obj");
	shapes.at(0) = tempShape;
	tempShape.load("../materials/bunny.obj");
	shapes.at(1) = tempShape;
	//Texture tex;
	//tex.setFilename(texfile);
	//textures.at(currentShape) = tex;
	shapespertex.at(0) = 1;

	Eigen::Vector3f T;
	T(0) = 0.0f;
	T(1) = -0.1f;
	T(2) = 0.0f;

	Eigen::Vector3f S;
	S(0) = 800.0f;
	S(1) = 0.2f;
	S(2) = 800.0f;

	WorldObject newObj(&shapes.at(0), S, T, Eigen::Vector3f(0.0f, 0.0f, 0.0f));

	objects.push_back(newObj);

	T << 0.0f, 1.0f, 0.0f;
	S << 1.0f, 1.0f, 1.0f;

	WorldObject newObj2(&shapes.at(0), S, T, Eigen::Vector3f(0.0f, 0.0f, 0.0f));

	objects.push_back(newObj2);
}

int Scene::getNumUncollectedObjects() {
	int num = 0;
	for (unsigned int i = 0; i < objects.size(); i++)
		if (!objects[i].getIsCollected())
			num++;

	// the stage is not an object to be referenced
	return num - 1;
}
