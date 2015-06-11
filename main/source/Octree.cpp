#include "Octree.hpp"
#include <GLFW/glfw3.h>
#include <stdlib.h>

Octree::Octree() {
    this->mid.x = this->mid.y = this->mid.z = 0;
    this->size.x = this->size.y = this->size.z = 0;

    for (int i = 0; i < NUM_CHILDREN; i++)
        children[i] = NULL;
}

Octree::~Octree() {
    for (int i = 0; i < NUM_CHILDREN; i++)
        delete children[i];
}

// no need to be accurate, only consistent
int Octree::getOctant(vec3 point) {
    int octant = 0;

    if (point.x >= mid.x) octant += 4;
    if (point.y >= mid.y) octant += 2;
    if (point.z >= mid.z) octant += 1;

    return octant;
}

void Octree::updateBounds(BoundingBox box) {
    this->updateBounds(box.min + box.size);
    this->updateBounds(box.min);
}

void Octree::updateBounds(vec3 point) {
    vec3 min, max;

    min = mid - size;
    max = mid + size;

    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);

    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);


    mid = (max + min) / 2.0f;
    size = (max - min) / 2.0f;
}

void Octree::insert(BoundingBox *box) {
    if (getOctant(box->min) == getOctant(box->min + box->size)) {
        if (this->isLeaf()) this->split();

        children[this->getOctant(box->min)]->insert(box);
    }
    else
        this->boxes.push_back(box);
}

std::vector<BoundingBox *> Octree::getCollisions(Actor *toCheck) {
    std::vector<BoundingBox *> colliding;

    for (int i = 0; i < toCheck->numShapes; i++) {
        std::vector<BoundingBox *> subcollisions = getCollisions(&toCheck->boxes[i]);
        colliding.insert(colliding.end(), subcollisions.begin(), subcollisions.end());
    }

    return colliding;
}

std::vector<BoundingBox *> Octree::getCollisions(BoundingBox *toCheck) {
    std::vector<BoundingBox *> colliding;
    for (size_t i = 0; i < boxes.size(); i++) {
        if (toCheck->collides(*boxes[i]))
            colliding.push_back(boxes[i]);

        if (!isLeaf())
	    for (int j = 0; j < NUM_CHILDREN; j++)
	        if (children[j]->containsPart(toCheck)) {
	            std::vector<BoundingBox *> tmp = children[j]->getCollisions(toCheck);
                    for (size_t k = 0; k < tmp.size(); k++)
                        colliding.push_back(tmp[k]);
	        }
    }

    return colliding;
}

void Octree::split() {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        vec3 tmp = size / 4.0f;

        if (!(i & 4)) tmp.x *= -1;
        if (!(i & 2)) tmp.y *= -1;
        if (!(i & 1)) tmp.z *= -1;

        children[i] = new Octree();
        children[i]->mid = tmp + mid;
        children[i]->size = size / 2.0f;
    }
}

bool Octree::containsPart(BoundingBox *toCheck) {
    BoundingBox tmp;

    tmp.insert(mid - size);
    tmp.insert(mid + size);

    return toCheck->collides(tmp);
}

void Octree::draw() {
    /*glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(P.topMatrix().data());
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(MV.topMatrix().data()); // to world space to camera space

    vec3 min = mid - size / 2.0f;
    vec3 max = mid + size / 2.0f;

    // Draw
    glBegin(GL_LINES);
    glVertex3f(min.x, min.y, min.z);
    glVertex3f(max.x, min.y, min.z);

    glVertex3f(min.x, min.y, min.z);
    glVertex3f(min.x, max.y, min.z);

    glVertex3f(min.x, min.y, min.z);
    glVertex3f(min.x, min.y, max.z);

    glVertex3f(min.x, min.y, max.z);
    glVertex3f(min.x, max.y, max.z);

    glVertex3f(min.x, max.y, min.z);
    glVertex3f(min.x, max.y, max.z);

    glVertex3f(min.x, max.y, min.z);
    glVertex3f(max.x, max.y, min.z);

    glVertex3f(max.x, min.y, min.z);
    glVertex3f(max.x, max.y, min.z);

    glVertex3f(min.x, min.y, max.z);
    glVertex3f(max.x, min.y, max.z);

    glVertex3f(max.x, min.y, min.z);
    glVertex3f(max.x, min.y, max.z);

    glVertex3f(max.x, max.y, min.z);
    glVertex3f(max.x, max.y, max.z);

    glVertex3f(min.x, max.y, max.z);
    glVertex3f(max.x, max.y, max.z);

    glVertex3f(max.x, min.y, max.z);
    glVertex3f(max.x, max.y, max.z);

    glEnd();
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);*/
}
