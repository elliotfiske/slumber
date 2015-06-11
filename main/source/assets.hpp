#ifndef assets_h
#define assets_h

#include "GLSL.h"
#include <string>
#include "shaders/shader.h"
#include "shaders/LightingShader.h"
#include "shaders/ShadowShader.h"
#include "shaders/FBOShader.h"
#include "shaders/ReflectShader.h"
#include "shaders/HUDShader.h"
#include "shaders/BillboardShader.h"
#include "shaders/SimpleShader.h"
#include <map>
#include "actor.hpp"
#include "Octree.hpp"
#include "BillboardActor.h"

#include <SFML/Audio.hpp>

using namespace std;

#define CurrAssets Assets::instance()

#ifdef XCODE_IS_TERRIBLE
    #define RESOURCE_FOLDER string("../resources/")
    #define MODELS_FOLDER string("../resources/models/")
    #define SOUND_FOLDER string("../resources/sounds")
#else
    #define RESOURCE_FOLDER string("resources/")
    #define MODELS_FOLDER string("resources/models/")
    #define SOUND_FOLDER string("resources/sounds")
#endif

class Assets {
public:
    /** Global "Assets" instance */
    static Assets* instance() {
        static Assets *instance = new Assets();
        return instance;
    }
    
    SimpleShader   *simpleShader;
    ShadowShader   *shadowShader;
    LightingShader *lightingShader;
    LightingShader *ghostLightingShader;
    BaseMVPShader  *collectibleShader;
    BillboardShader *billboardShader;
    HUDShader *hudShader;
    
    FBOShader      *currShader;
    FBOShader      *motionBlurShader;
    FBOShader      *ghostShader;
    FBOShader      *woozyShader;

    Octree *octree;
    ReflectShader  *reflectionShader;
    
    FBOShader      *currFBOShader;
    
//    void doPlay(string filename);
    void sendShapeToGPU(tinyobj::shape_t shape, tinyobj::material_t material, Actor *actor, int shapeNdx);
    void play(string filename, vec3 pos = vec3(0.0f, 0.0f, 0.0f));
    void stopSounds();
    
    // A simple dictionary where the key is the OBJ name and the
    //  value is the Actor instance that uses that model.
    std::map<string, Actor*>  actorDictionary;
    
    std::map<string, BillboardActor*> billboardDictionary;
    
    Actor *masterBillboard;
    
private:
    Assets();
    void loadShape(string filename, Actor *actor);
    void readLevelData(string filename);
    void generateBillboards(string filename);
//    void loadSoundBuffer(string filename);
    
    map<string, sf::SoundBuffer> soundBuffers;
    vector<sf::Sound> sounds;
};


#endif
