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

#ifdef THREADS
    #include <thread>
#endif

using namespace std;

/**
 * Init all the shapes, materials (TODO), and shaders we need
 *  for our game
 */
Assets::Assets() {
    billboardShader   = new LightingShader("Billboard_Vert.glsl", "Billboard_Frag.glsl");
    hudShader         = new LightingShader("HUD_Vert.glsl", "HUD_Frag.glsl");
    lightingShader    = new LightingShader("Lighting_Vert.glsl", "Lighting_PCSS_Frag.glsl");
    ghostLightingShader = new LightingShader("Lighting_Vert.glsl", "Lighting_PCSS_Frag_Ghost.glsl");
    
    ghostShader       = new FBOShader("FBO_Vert.glsl", "FBO_Frag_Ghost_Vision.glsl");
    currShader        = new FBOShader("FBO_Vert.glsl", "FBO_Frag_Darken.glsl");
    motionBlurShader  = new FBOShader("FBO_Vert.glsl" , "FBO_Frag_Motion_Blur.glsl");
    woozyShader       = new FBOShader("FBO_Vert.glsl", "FBO_Frag_Woozy.glsl");
    
    collectibleShader = new BaseMVPShader("Collectible_Vert.glsl", "Collectible_Frag.glsl");
    
    shadowShader      = new ShadowShader("Shadow_Vert.glsl", "Shadow_Frag.glsl");
    reflectionShader  = new ReflectShader("Reflection_Vert.glsl", "Reflection_Frag.glsl");
    
    string levelDataName = RESOURCE_FOLDER + string("level.txt");
    readLevelData(levelDataName);
    
    string billboardsName = RESOURCE_FOLDER + string("billboards.txt");
    generateBillboards(billboardsName);

    // TODO: pre-load sounds by calling this->loadSoundBuffer(<filename>);
}

/**
 * Populate the actorDictionary with information from the level file
 */
void Assets::readLevelData(string filename) {
    ifstream levelFile(filename.c_str());
    if (!levelFile.is_open()) {
        cerr << "Couldn't open level data with filename " << filename << endl;
        return;
    }
    
    string currActorName;
    while (levelFile >> currActorName) {
        float actorAngle;
        
        vec3 newActorCenter;
        levelFile >> newActorCenter.x;
        levelFile >> newActorCenter.y;
        levelFile >> newActorCenter.z;
        levelFile >> actorAngle;
        
        actorDictionary[currActorName] = new Actor(newActorCenter);
        string objFilename(MODELS_FOLDER + currActorName + ".obj");
        loadShape(objFilename, actorDictionary[currActorName]);
        
        actorDictionary[currActorName]->direction.y = actorAngle;
    }
}

/**
 * Load all the billboards and their respective textures
 */
void Assets::generateBillboards(string filename) {
    
    // First, load the billboard's shape (a plane) into the GPU
    masterBillboard = new Actor(vec3(0, 0, 0));
    loadShape(MODELS_FOLDER + "plane.obj", masterBillboard);
    
    ifstream billboardFile(filename.c_str());
    if (!billboardFile.is_open()) {
        cerr << "Couldn't open level data with filename " << filename << endl;
        return;
    }
    
    string currBillboardName;
    while (billboardFile >> currBillboardName) {
        vec3 billboardCenter;
        float billboardAngle, billboardScale;
        
        billboardFile >> billboardCenter.x;
        billboardFile >> billboardCenter.y;
        billboardFile >> billboardCenter.z;
        billboardFile >> billboardAngle;
        billboardFile >> billboardScale;
        
        BillboardActor *billy = new BillboardActor(billboardCenter, billboardScale, billboardAngle, masterBillboard);
        
        Texture *billboardTexture = new Texture();
        billboardTexture->setFilename(MODELS_FOLDER + "billboards/" + currBillboardName);
        billboardTexture->init(true);
        
        billy->texture[0] = billboardTexture;
        billy->material[0].diffuse_texname = currBillboardName;
        
        billboardDictionary[currBillboardName] = billy;
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
            if (existingTextures[ndx]->filename == MODELS_FOLDER + material.diffuse_texname) {
                actor->texture[shapeNdx] = existingTextures[ndx];
                textureAlreadyLoaded = true;
            }
        }
        
        if (!textureAlreadyLoaded) {
            actor->texture[shapeNdx] = new Texture();
            actor->texture[shapeNdx]->setFilename(MODELS_FOLDER + material.diffuse_texname);
            actor->texture[shapeNdx]->init(false);
            actor->textureUnit[shapeNdx] = textureUnit++;
            
            existingTextures.push_back(actor->texture[shapeNdx]);
        }
    }
    
    actor->numVerts[shapeNdx] = shape.mesh.indices.size();
    actor->material[shapeNdx] = material;
}

using namespace sf;

SoundBuffer loadSoundBuffer(string filename) {
    sf::SoundBuffer buf;
    buf.loadFromFile(filename);
    //    soundBuffers[filename] = buf;
    return buf;
}

string filename;
thread *wut;
bool killSound = false;


void doPlay() {
//    if (soundBuffers.find(filename) == soundBuffers.end())
//        this->loadSoundBuffer(filename);
    
    sf::SoundBuffer buf = loadSoundBuffer(filename);
    sf::Sound sound(buf);
    
//    sound.setPosition(sf::Vector3f(pos.x, pos.y, pos.z));
    sound.play();
    killSound = false;
    
    while (sound.getStatus() == sf::Sound::Playing && !killSound) { }
}


void Assets::play(string filename_, vec3 pos) {
#ifdef THREADS

    filename = filename_;
    killSound = true;
    wut = new thread(doPlay);

#endif
}

void Assets::stopSounds() {
    killSound = true;
}

/**
 * Sends .OBJ data to the GPU and tells the actor what its
 *  array IDs are.
 */
void Assets::loadShape(string filename, Actor *actor) {
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err = tinyobj::LoadObj(shapes, materials, filename.c_str(), MODELS_FOLDER.c_str());
    if(!err.empty()) {
        printf("OBJ error: %s\n", err.c_str());
    }
    
    for (int ndx = 0; ndx < shapes.size(); ndx++) {
        tinyobj::material_t currMaterial = materials[shapes[ndx].mesh.material_ids[0]];
        sendShapeToGPU(shapes[ndx], currMaterial, actor, ndx);
        
        // HACKITY HACK HACK: grab the tv screen so we can apply the special static texture to it
        if (shapes[ndx].name == "SCREEN") {
            printf("Gotcha\n");
            actor->tvScreenIndex = ndx;
        }
    }
    
    actor->numShapes = shapes.size();
}


