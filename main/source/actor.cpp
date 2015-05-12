#include "actor.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

Actor::Actor(vec3 center_) {
    center = center_;
}

void Actor::step(double dt) {
    vec3 curChange;
    curChange = (velocityScalar * (float) dt) * direction;
    center += curChange;
}

bool Actor::detectIntersect(Actor target, bool overrideCooldown) {
    // TODO: this
    return false;
}

void Actor::setModel() {
    glm::mat4 Trans  = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 RotX   = glm::rotate(glm::mat4(1.0f), direction.x, vec3(1, 0, 0));
    glm::mat4 RotY   = glm::rotate(glm::mat4(1.0f), direction.y, vec3(0, 1, 0));
    glm::mat4 RotZ   = glm::rotate(glm::mat4(1.0f), direction.z, vec3(0, 0, 1));
    
    glm::mat4 Composite = Trans * RotX * RotY * RotZ;
    
    modelMat = Composite;
    
    CurrAssets->lightingShader->setModelMatrix(Composite);
}

void Actor::setMaterial(tinyobj::material_t material) {
    CurrAssets->lightingShader->setAmbientColor(material.ambient);
    CurrAssets->lightingShader->setDiffuseColor(material.diffuse);
    CurrAssets->lightingShader->setSpecularColor(material.specular);
    CurrAssets->lightingShader->setShininess(10.0);
}

void Actor::draw(Light *light) {
    setModel();
    setLightMVP(light, false);
    
    for (int ndx = 0; ndx < numShapes; ndx++) {
        setMaterial(material[ndx]);
        CurrAssets->lightingShader->setPositionArray(posID[ndx]);
        CurrAssets->lightingShader->setNormalArray(norID[ndx]);
        CurrAssets->lightingShader->setIndexArray(indID[ndx]);
        
        // Texture stuff
        if (material[ndx].diffuse_texname.size() > 0) {
            CurrAssets->lightingShader->setUVArray(uvID[ndx]);
            texture[ndx]->bind(CurrAssets->lightingShader->diffuseTexture_UniformID, textureUnit[ndx]);
        }
        
        glDrawElements(GL_TRIANGLES, numVerts[ndx], GL_UNSIGNED_INT, (void*) 0);
        
        if (material[ndx].diffuse_texname.size() > 0) {
            texture[ndx]->unbind(textureUnit[ndx]);
        }
    }
}

void Actor::setLightMVP(Light *light, bool isShadowShader) {
    glm::mat4 P = light->getProjectionMatrix();
    glm::mat4 V = light->getViewMatrix();

    glm::mat4 Trans  = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 RotX   = glm::rotate(glm::mat4(1.0f), direction.x, vec3(1, 0, 0));
    glm::mat4 RotY   = glm::rotate(glm::mat4(1.0f), direction.y, vec3(0, 1, 0));
    glm::mat4 RotZ   = glm::rotate(glm::mat4(1.0f), direction.z, vec3(0, 0, 1));
    
    glm::mat4 M = Trans * RotX * RotY * RotZ;
    glm::mat4 MVP = P * V * M;
    
    if (isShadowShader) {
        CurrAssets->shadowShader->setMVPmatrix(MVP);
    }
    else {
        CurrAssets->lightingShader->setLightPos(light->getPosition());
        CurrAssets->lightingShader->setLightMVP(MVP);
    }
}

void Actor::drawShadows(Light *light) {
    setLightMVP(light, true);

//    CurrAssets->shadowShader->setPositionArray(posID);
//
//    glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, (void*) 0);
}
