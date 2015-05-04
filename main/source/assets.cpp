#include "tiny_obj_loader.h"
#include "GLSL.h"
#include "assets.hpp"
#include "actor.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cassert>
#include <iostream>

using namespace std;

/**
 * Init all the shapes, materials (TODO), and shaders we need
 *  for our game
 */
Assets::Assets() {
    lightingShader = new LightingShader("Lighting_Vert.glsl", "Lighting_Frag.glsl");
    darkeningShader = new FBOShader("FBO_Vert.glsl", "FBO_Frag_Darken.glsl");
    
    // Populate the levelDict variable
    readLevelData("level.txt");
}

/**
 * Populate the levelDict with information from the level file
 */
void Assets::readLevelData(string filename) {
    ifstream levelFile(filename);
    if (!levelFile.is_open()) {
        cerr << "Couldn't open level data with filename " << filename << endl;
        return;
    }
    
    string currActorName;
    while (levelFile >> currActorName) {
        vec3 newActorCenter;
        levelFile >> newActorCenter.x;
        levelFile >> newActorCenter.y;
        levelFile >> newActorCenter.z;
        
        levelDict[currActorName] = newActorCenter;
    }
}

void Assets::loadShape(const char* filename, GLuint *posID, GLuint *norID, GLuint *indID, int *numVerts) {
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err = tinyobj::LoadObj(shapes, materials, filename);
    if(!err.empty()) {
        printf("OBJ error: %s\n", err.c_str());
    }
    
    const vector<float> &posBuf = shapes[0].mesh.positions;
    glGenBuffers(1, posID);
    glBindBuffer(GL_ARRAY_BUFFER, *posID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    // Send the normal array to the GPU
    const vector<float> &norBuf = shapes[0].mesh.normals;
    glGenBuffers(1, norID);
    glBindBuffer(GL_ARRAY_BUFFER, *norID);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    
    // Send the index array to the GPU
    const vector<unsigned int> &indBuf = shapes[0].mesh.indices;
    glGenBuffers(1, indID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    
    *numVerts = shapes[0].mesh.indices.size();
}

/**
 * Uses the levelDict to create an actor with the correct
 *  OBJ data and position
 */
Actor Assets::actorFromName(string actorName) {
    return Actor(vec3(0, 0, 0));
}


