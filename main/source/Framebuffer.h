//#ifndef _FRAMEBUFFER_H_
//#define _FRAMEBUFFER_H_

#include "GLSL.h"

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	// if not called, this buffer will act as the default window buffer
	void generate();

	// generates and attaches a texture to this framebuffer
	// returns status ok flag
	bool generateTexture(GLuint width, GLuint height, bool clamp = true);
	bool generateShadowTexture(GLuint width, GLuint height, bool clamp = true);

	void bind();

	void bindTexture(GLint tex_handle, GLint unit = 0);

	// unit must be the same as bind
	void unbind();

	void unbindTexture();

private:

	GLuint id;
	GLuint tex_id;
	GLuint width, height;
	GLint unit;
};

//#endif//_FRAMEBUFFER_H_
