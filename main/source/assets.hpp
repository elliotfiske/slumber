#ifndef assets_h
#define assets_h

#include "GLSL.h"
#include <string>
#include "shader.h"
#include <map>
#include "actor.hpp"

using namespace std;

#define CurrAssets Assets::instance()

#ifdef XCODE_IS_TERRIBLE
    #define RESOURCE_FOLDER "../resources/models/"
#else
    #define RESOURCE_FOLDER "resources/models/"
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
    FBOShader      *darkeningShader;
    FBOShader      *motionBlurShader;
    
    Actor* actorFromName(string actorName);
    void sendShapeToGPU(tinyobj::shape_t shape, tinyobj::material_t material, Actor *actor, int shapeNdx);
    
private:
    Assets();
    void loadShape(string filename, Actor *actor);
    void readLevelData(string filename);
    
    // A simple dictionary where the key is the OBJ name and the
    //  value is the position of that model.
    std::map<string, glm::vec3>  levelDict;
};


#endif
