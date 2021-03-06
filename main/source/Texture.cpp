#include "Texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() :
filename(""),
tid(0)
{
    
}

Texture::~Texture()
{
    
}

void Texture::init(bool nearFilter = false)
{
    // Load texture
    int w, h, ncomps;
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &ncomps, 0);
    if(!data) {
        std::cerr << filename << " not found" << std::endl;
    }
    
    GLenum textureChannels = GL_RGB;
    
    if (ncomps == 4) {
        textureChannels = GL_RGBA;
    }
    else if (ncomps == 3) {
        textureChannels = GL_RGB;
    }
    else {
        std::cerr << filename << " must have 3 components (RGB)" << std::endl;
    }
    
    if((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
        std::cerr << filename << " must be a power of 2" << std::endl;
    }
    width = w;
    height = h;
    
    // Generate a texture buffer object
    glGenTextures(1, &tid);
    // Bind the current texture to be the newly generated texture object
    glBindTexture(GL_TEXTURE_2D, tid);
    // Load the actual texture data
    // Base level is 0, number of channels is 3, and border is 0.
    glTexImage2D(GL_TEXTURE_2D, 0, ncomps, width, height, 0, textureChannels, GL_UNSIGNED_BYTE, data);
    // Generate image pyramid
    glGenerateMipmap(GL_TEXTURE_2D);
    // Set texture wrap modes for the S and T directions
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set filtering mode for magnification and minimification
    if (nearFilter) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    }
    else {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free image, since the data is now on the GPU
    stbi_image_free(data);
}

void Texture::bind(GLint handle, GLint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
    glUniform1i(handle, unit);
}

void Texture::unbind(GLint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
