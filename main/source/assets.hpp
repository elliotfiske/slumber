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

using namespace std;

#define CurrAssets Assets::instance()

#ifdef XCODE_IS_TERRIBLE
    #define RESOURCE_FOLDER "../resources/"
    #define MODELS_FOLDER "../resources/models/"
#else
    #define RESOURCE_FOLDER "resources/"
    #define MODELS_FOLDER "../resources/"
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
    BaseMVPShader  *collectibleShader;
    FBOShader      *darkeningShader;
    FBOShader      *motionBlurShader;
    
    void sendShapeToGPU(tinyobj::shape_t shape, tinyobj::material_t material, Actor *actor, int shapeNdx);
    
    // A simple dictionary where the key is the OBJ name and the
    //  value is the Actor instance that uses that model.
    std::map<string, Actor*>  actorDictionary;
    
private:
    Assets();
    void loadShape(string filename, Actor *actor);
    void readLevelData(string filename);
    
};


#endif
