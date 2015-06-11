#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <vector>
#include "BoundingBox.hpp"
#include "camera.hpp"

#define NUM_CHILDREN 8

class Octree {
public:
    Octree();
    ~Octree();
    bool isLeaf() { return children[0] == NULL; }
    void draw();
    void updateBounds(BoundingBox box);
    void insert(BoundingBox *box);
    std::vector<BoundingBox *> getCollisions(Camera *camera);
    std::vector<BoundingBox *> getCollisions(BoundingBox *toCheck);

private:
    int getOctant(vec3 point);
    void split();
    bool containsPart(BoundingBox *toCheck);
    void updateBounds(vec3 point);

    vec3 mid, size;
    Octree *children[NUM_CHILDREN];
    std::vector<BoundingBox *> boxes;
};

#endif
