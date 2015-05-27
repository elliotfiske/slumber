#ifndef assets_h
#define assets_h

#include "GLSL.h"
#include <string>
#include "shaders/shader.h"
#include "shaders/LightingShader.h"
#include "shaders/ShadowShader.h"
#include "shaders/FBOShader.h"
#include <map>
#include "actor.hpp"
#include "BillboardActor.h"

#include <SFML/Audio.hpp>

using namespace std;

#define CurrAssets Assets::instance()

#ifdef XCODE_IS_TERRIBLE
    #define RESOURCE_FOLDER "../resources/"
    #define MODELS_FOLDER string("../resources/models/")
#else
    #define RESOURCE_FOLDER "resources/"
    #define MODELS_FOLDER string("resources/models/")
#endif

class Assets {
public:
    /** Global "Assets" instance */
    static Assets* instance() {
        static Assets *instance = new Assets();
        return instance;
    }
    
    ShadowShader   *shadowShader;
    LightingShader *lightingShader;
    LightingShader *ghostLightingShader;
    BaseMVPShader  *collectibleShader;
    LightingShader  *billboardShader;
    
    FBOShader      *currShader;
    FBOShader      *motionBlurShader;
    FBOShader      *ghostShader;
    FBOShader      *woozyShader;
    
    FBOShader      *currFBOShader;
    
    void sendShapeToGPU(tinyobj::shape_t shape, tinyobj::material_t material, Actor *actor, int shapeNdx);
    void play(string filename, vec3 pos = vec3(0.0f, 0.0f, 0.0f));
    
    // A simple dictionary where the key is the OBJ name and the
    //  value is the Actor instance that uses that model.
    std::map<string, Actor*>  actorDictionary;
    
    std::map<string, BillboardActor*> billboardDictionary;
    
private:
    Assets();
    void loadShape(string filename, Actor *actor);
    void readLevelData(string filename);
    void generateBillboards(string filename);
    void loadSoundBuffer(string filename);
    
    map<string, sf::SoundBuffer> soundBuffers;
    vector<sf::Sound> sounds;
};


#endif
