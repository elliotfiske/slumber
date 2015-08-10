#include "actor.hpp"
#include "GLSL.h"
#include "assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <cmath>
#include <iostream>

float catmull[] = {
    0.0f, -1.0f,  2.0f, -1.0f,
    2.0f,  0.0f, -5.0f,  3.0f,
    0.0f,  1.0f,  4.0f, -3.0f,
    0.0f,  0.0f, -1.0f,  1.0f
};

Actor::Actor(vec3 center_) {
    orig_center = center = center_;
    scale = 1.0;
    
    animate = false;
    animation_time = 0;
    boundSphereRad = 5.0;
}

void Actor::step(double dt) {
    mat4 B = 0.5f * glm::transpose(glm::make_mat4(catmull));
    
    if (animate && cps.size() >= 4) {
        animation_time += dt;
        float kfloat;
        float u = std::modf(std::fmod(animation_time, cps.size() - 3.0f), &kfloat);
        int k = (int)std::floor(kfloat);
        
        mat4 G;
        vec4 centerPt = vec4(orig_center.x, orig_center.y, orig_center.z, 0.0);
        for (int i = 0; i < 4; i++)
            G[i] = cps[k + i] + centerPt;
        
        // u in [0,1]
        // k in [1,ncps-3]
        vec4 uVec(1.0f, u, u * u, u * u * u);
        vec4 pos = G * B * uVec;
        
        this->center = vec3(pos);
    }
    else {
        animation_time = 0;
        this->center = orig_center;
    }
}

bool Actor::detectIntersect(Actor target, bool overrideCooldown) {
    // TODO: this
    return false;
}

void Actor::setModel() {
    glm::mat4 Scale  = glm::scale(glm::mat4(1.0f), vec3(scale, scale, scale));
    glm::mat4 Trans  = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 RotX   = glm::rotate(glm::mat4(1.0f), direction.x, vec3(1, 0, 0));
    glm::mat4 RotY   = glm::rotate(glm::mat4(1.0f), direction.y, vec3(0, 1, 0));
    glm::mat4 RotZ   = glm::rotate(glm::mat4(1.0f), direction.z, vec3(0, 0, 1));
    
    glm::mat4 Composite = Trans * RotX * RotY * RotZ * Scale;

    modelMat = Composite;
    
    CurrAssets->lightingShader->setModelMatrix(Composite);
}

void Actor::setMaterial(tinyobj::material_t material) {
    CurrAssets->lightingShader->setAmbientColor(material.ambient);
    CurrAssets->lightingShader->setDiffuseColor(material.diffuse);
    CurrAssets->lightingShader->setSpecularColor(material.specular);
    CurrAssets->lightingShader->setShininess(material.shininess);
}

void Actor::draw(Light *light, bool tv) {
    setModel();
    setLightMVP(light, false);
    
    for (int ndx = 0; ndx < numShapes; ndx++) {
//        if (tv && ndx == glowingShapeIndex) {
//            material[ndx].ambient[0] = 255.0;
//            material[ndx].ambient[1] = 255.0;
//            material[ndx].ambient[2] = 255.0;
//        }
        
        setMaterial(material[ndx]);
        CurrAssets->lightingShader->setPositionArray(posID[ndx]);
        CurrAssets->lightingShader->setNormalArray(norID[ndx]);
        CurrAssets->lightingShader->setIndexArray(indID[ndx]);
        
        // Texture stuff
        if (material[ndx].diffuse_texname.size() > 0) {
            CurrAssets->lightingShader->setUVArray(uvID[ndx]);
            texture[ndx]->bind(CurrAssets->lightingShader->diffuseTexture_UniformID, 0);
        }
        
        glDrawElements(GL_TRIANGLES, numVerts[ndx], GL_UNSIGNED_INT, (void*) 0);
        
        if (material[ndx].diffuse_texname.size() > 0) {
            texture[ndx]->unbind(0);
        }
    }
}

void Actor::setLightMVP(Light *light, bool isShadowShader) {
    glm::mat4 P = light->getProjectionMatrix();
    glm::mat4 V = light->getViewMatrix();

    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
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

    for (int ndx = 0; ndx < numShapes; ndx++) {
        CurrAssets->shadowShader->setPositionArray(posID[ndx]);
        CurrAssets->shadowShader->setIndexArray(indID[ndx]);
        
        glDrawElements(GL_TRIANGLES, numVerts[ndx], GL_UNSIGNED_INT, 0);
    }
}


