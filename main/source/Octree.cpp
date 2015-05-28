#include <Octree.hpp>
#include <stdlib.h>

Octree::Octree() {
    for (int i = 0; i < NUM_CHILDREN; i++)
        children[i] = NULL;
}

Octree::Octree(vec3 mid, vec3 size) {
    this->mid = mid;
    this->size = size;

    for (int i = 0; i < NUM_CHILDREN; i++)
        children[i] = NULL;
}

Octree::~Octree() {
    for (int i = 0; i < NUM_CHILDREN; i++)
        delete chidren[i];
}

// no need to be accurate, only consistent
int Octree::getOctant(vec3 point) {
    int octant = 0;

    if (point.x >= mid.x) octant += 4;
    if (point.y >= mid.y) octant += 2;
    if (point.z >= mid.z) octant += 1;

    return octant;
}

void Octree::insert(Actor actor) {
    if (abs(actor.center.x - mid.x) > actor.boundSphereRad &&
        abs(actor.center.y - mid.y) > actor.boundSphereRad &&
        abs(actor.center.z - mid.z) > actor.boundSphereRad) {
        if (this->isLeaf()) this->split();

        children[this->getOctant(actor.center)]->insert(actor);
    }
    else
        this->actors.push_back(actor);
}

std::vector<Actor> Octree::getCollisions(Actor toCheck) {
    
}

void Octree::split() {
    for (int i = 0; i < NUM_CHILDREN; i++) 
        vec3 tmp = size / 4.0f;

        if (!(i & 4)) tmp.x *= -1;
        if (!(i & 2)) tmp.y *= -1;
        if (!(i & 1)) tmp.z *= -1;

        children[i] = new Octree(tmp + mid, size / 2.0f);
    }
}

void Octree::draw() {
    glMatrixMode(GL_PROJECTION);
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
    glMatrixMode(GL_MODELVIEW);
}
