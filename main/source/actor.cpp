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
    
    CurrAssets->lightingShader->setModelMatrix(Composite);
}

void Actor::setMaterial() {
    CurrAssets->lightingShader->setAmbientColor(ambientColor);
    CurrAssets->lightingShader->setDiffuseColor(diffuseColor);
    CurrAssets->lightingShader->setSpecularColor(specularColor);
    CurrAssets->lightingShader->setShininess(shininess);
}

void Actor::draw(Light *light) {
    setModel();
    setMaterial();
    setLightMVP(light, false);
    
    CurrAssets->lightingShader->setPositionArray(posID);
    CurrAssets->lightingShader->setNormalArray(norID);
    CurrAssets->lightingShader->setIndexArray(indID);
    
    glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, (void*) 0);
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

    CurrAssets->shadowShader->setPositionArray(posID);

    glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, (void*) 0);
}
