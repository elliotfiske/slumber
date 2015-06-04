//
//  BillboardActor.cpp
//  slumber
//
//  Created by Elliot Fiske on 5/26/15.
//
//

#include "BillboardActor.h"
#include "assets.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" //value_ptr

/**
 * Construct a new BillboardActor, telling it what texture to display, where to
 *  show it, and how to rotate (only along the Y-axis)
 *
 * Takes in an actor "master billboard" and copies the location of the plane w
 *  sent to the GPU earlier
 */
BillboardActor::BillboardActor(vec3 center_, float scale_, float angle, Actor *masterBillboard): Actor(center_) {
    posID[0]    = masterBillboard->posID[0];
    norID[0]    = masterBillboard->norID[0];
    indID[0]    = masterBillboard->indID[0];
    uvID[0]     = masterBillboard->uvID[0];
    numVerts[0] = masterBillboard->numVerts[0];
    numShapes = 1;
    
    direction.y = angle;
    scale = scale_;
}

void BillboardActor::setModel() {
    glm::mat4 Scale  = glm::scale(glm::mat4(1.0f), vec3(scale, scale, scale));
    glm::mat4 Trans  = glm::translate(glm::mat4(1.0f), center);
    glm::mat4 RotX   = glm::rotate(glm::mat4(1.0f), direction.x, vec3(1, 0, 0));
    glm::mat4 RotY   = glm::rotate(glm::mat4(1.0f), direction.y, vec3(0, 1, 0));
    glm::mat4 RotZ   = glm::rotate(glm::mat4(1.0f), direction.z, vec3(0, 0, 1));
    
    glm::mat4 Composite = Trans * RotX * RotY * RotZ * Scale;
    
    modelMat = Composite;
    
    CurrAssets->billboardShader->setModelMatrix(Composite);
}


void BillboardActor::draw(Light *light, bool tv) {
    setModel();
    GLenum err;
    
    for (int ndx = 0; ndx < numShapes; ndx++) {
        
//        setMaterial(material[ndx]); TODO: stick the material back on and use it for ui hilighting
        CurrAssets->billboardShader->setPositionArray(posID[ndx]);
        CurrAssets->billboardShader->setIndexArray(indID[ndx]);
        
        // Texture stuff
        if (material[ndx].diffuse_texname.size() > 0) {
            CurrAssets->billboardShader->setUVArray(uvID[ndx]);
            texture[ndx]->bind(CurrAssets->billboardShader->diffuseTexture_UniformID, 0);
            while ((err = glGetError()) != GL_NO_ERROR) {
                cerr << "5 OpenGL error from Title state: " << err << endl;
            }
            
        }
        
        glDrawElements(GL_TRIANGLES, numVerts[ndx], GL_UNSIGNED_INT, (void*) 0);
        
        if (material[ndx].diffuse_texname.size() > 0) {
            texture[ndx]->unbind(0);
        }
    }
}
