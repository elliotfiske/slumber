#include "Framebuffer.h"

Framebuffer::Framebuffer() :
	id(0),
	tex_id(0),
	width(0),
	height(0)
{
    
}

Framebuffer::~Framebuffer() {
}

void Framebuffer::generate() {
	glGenFramebuffers(1, &id);
}

bool Framebuffer::generateShadowTexture(GLuint width, GLuint height, bool clamp) {
	GLint tmp = 0, tmp_tex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmp_tex);

	this->width = width;
	this->height = height;

	glGenTextures(1, &tex_id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (clamp) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	bool check_status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindTexture(GL_TEXTURE_2D, tmp_tex);
	glBindFramebuffer(GL_FRAMEBUFFER, tmp);

	return check_status;
}

bool Framebuffer::generateTexture(GLuint width, GLuint height, bool clamp) {
//	GLint tmp = 0, tmp_tex = 0;
//	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmp_tex);
//
//	this->width = width;
//	this->height = height;
//
//	glGenTextures(1, &tex_id);
//	glBindFramebuffer(GL_FRAMEBUFFER, id);
//	glBindTexture(GL_TEXTURE_2D, tex_id);
//    
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	if (clamp) {
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	}
//	else {
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	}
//
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_id, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//
//	bool check_status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
//
//	glBindTexture(GL_TEXTURE_2D, tmp_tex);
//	glBindFramebuffer(GL_FRAMEBUFFER, tmp);
//
//	return check_status;
    
    
    this->width = width;
    this->height = height;
    
    glGenTextures(1, &tex_id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, tex_id);
    
    // Give an empty image to OpenGL ( the last "0" means "empty" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    
    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    
    return true;
}

void Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::bindTexture(GLint handle, GLint unit) {
	if (tex_id != 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glUniform1i(handle, unit);
		this->unit = unit;
	}
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::unbindTexture() {
	if (tex_id != 0) {	
		glActiveTexture(GL_TEXTURE0 + this->unit);
		glBindTexture(GL_TEXTURE_2D, 0);
		this->unit = 0;
	}
}
