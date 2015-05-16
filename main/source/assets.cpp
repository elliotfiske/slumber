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
    collectibleShader = new BaseMVPShader("Collectible_Vert.glsl", "Collectible_Frag.glsl");
    darkeningShader = new FBOShader("FBO_Vert.glsl", "FBO_Frag_Darken.glsl");
    motionBlurShader = new FBOShader("FBO_Vert.glsl" , "FBO_Frag_Motion_Blur.glsl");
    shadowShader = new ShadowShader("Shadow_Vert.glsl", "Shadow_Frag.glsl");
    
    string levelDataName = "resources/level.txt";
    
#ifdef XCODE_IS_TERRIBLE
    levelDataName = "../" + levelDataName;
#endif
    
    readLevelData(levelDataName);
}

/**
 * Populate the levelDict with information from the level file
 */
void Assets::readLevelData(string filename) {
    ifstream levelFile(filename.c_str());
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

vector<Texture *> existingTextures;

/**
 * Takes a shape and buffers the position, normals, index and 
 *  UV's (if they exist) to the GPU
 */
void Assets::sendShapeToGPU(tinyobj::shape_t shape, tinyobj::material_t material, Actor *actor, int shapeNdx) {
    static GLuint textureUnit = 3;

    const vector<float> &posBuf = shape.mesh.positions;
    glGenBuffers(1, &actor->posID[shapeNdx]);
    glBindBuffer(GL_ARRAY_BUFFER, actor->posID[shapeNdx]);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
    
    // Send the normal array to the GPU
    const vector<float> &norBuf = shape.mesh.normals;
    glGenBuffers(1, &actor->norID[shapeNdx]);
    glBindBuffer(GL_ARRAY_BUFFER, actor->norID[shapeNdx]);
    glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
    
    // Send the index array to the GPU
    const vector<unsigned int> &indBuf = shape.mesh.indices;
    glGenBuffers(1, &actor->indID[shapeNdx]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, actor->indID[shapeNdx]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indBuf.size()*sizeof(unsigned int), &indBuf[0], GL_STATIC_DRAW);
    
    // Send the UV array to the GPU
    if (material.diffuse_texname.size() > 0) {
        const vector<float> &uvBuf = shape.mesh.texcoords;
        glGenBuffers(1, &actor->uvID[shapeNdx]);
        glBindBuffer(GL_ARRAY_BUFFER, actor->uvID[shapeNdx]);
        glBufferData(GL_ARRAY_BUFFER, uvBuf.size()*sizeof(float), &uvBuf[0], GL_STATIC_DRAW);
        
        // If we already loaded a texture, don't load it again!
        bool textureAlreadyLoaded = false;
        for (int ndx = 0; ndx < existingTextures.size(); ndx++) {
            if (existingTextures[ndx]->filename == RESOURCE_FOLDER + material.diffuse_texname) {
                actor->texture[shapeNdx] = existingTextures[ndx];
                textureAlreadyLoaded = true;
            }
        }
        
        if (!textureAlreadyLoaded) {
            actor->texture[shapeNdx] = new Texture();
            actor->texture[shapeNdx]->setFilename(RESOURCE_FOLDER + material.diffuse_texname);
            actor->texture[shapeNdx]->init();
            actor->textureUnit[shapeNdx] = textureUnit++;
            
            existingTextures.push_back(actor->texture[shapeNdx]);
        }
    }
    
    actor->numVerts[shapeNdx] = shape.mesh.indices.size();
    actor->material[shapeNdx] = material;
}



/**
 * Sends .OBJ data to the GPU and tells the actor what its
 *  array IDs are.
 */
void Assets::loadShape(string filename, Actor *actor) {
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), RESOURCE_FOLDER);
    if(!err.empty()) {
        printf("OBJ error: %s\n", err.c_str());
    }
    
    for (int ndx = 0; ndx < shapes.size(); ndx++) {
        tinyobj::material_t currMaterial = materials[shapes[ndx].mesh.material_ids[0]];
        sendShapeToGPU(shapes[ndx], currMaterial, actor, ndx);
    }
    
    actor->numShapes = shapes.size();
}



/**
 * Uses the levelDict to create an actor with the correct
 *  OBJ data and position
 */
Actor* Assets::actorFromName(string actorName) {
    Actor *result;
    
    result = new Actor(levelDict[actorName]);
    string objFilename("resources/models/" + actorName + ".obj");
    
#ifdef XCODE_IS_TERRIBLE
    objFilename = "../" + objFilename;
#endif
    
    loadShape(objFilename, result);
    
    return result;
}


