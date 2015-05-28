#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <vector>
#include "actor.hpp"

#define NUM_CHILDREN 8

class Octree {
    vec3 mid, size;
    Octree *children[NUM_CHILDREN];
    std::vector<Actor> actors;

public:
    Octree();
    ~Octree();
    bool isLeaf() { return children[0] == NULL; }
    void draw();
    void insert(Actor actor);
    std::vector<Actor> getCollisions(Actor toCheck);

private:
    int getOctant(vec3 point);
    void split();

    Octree(vec3 mid, vec3 size);
    Octree *children[NUM_CHILDREN];
    vec3 mid, size;
};

#endif
