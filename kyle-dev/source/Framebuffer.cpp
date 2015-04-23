#include "Framebuffer.h"

Framebuffer::Framebuffer() :
	id(0),
	text_id(0),
	width(0),
	height(0)
{
}

Framebuffer::~Framebuffer() {
}

bool Framebuffer::generate() {
	glGenFramebuffers(1, &id);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

bool Framebuffer::generateTexture(GLuint width, GLuint height, bool clamp) {
	GLuint tmp = 0, tmp_tex = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &tmp);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &tmp_tex);

	this->width = width;
	this->height = height;

	glGenTextures(1, &tex_id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

void Framebuffer::bind(GLint unit) {
	glBindFramebuffer(GL_FRAMEBUFFER, id);

	if (tex_id != 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, tid);

		// store unit for unbind
		this->unit = unit;
	}
}

void Framebuffer::bindTexture(GLint handle, GLint unit) {
	if (tex_id != 0) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, tid);
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
