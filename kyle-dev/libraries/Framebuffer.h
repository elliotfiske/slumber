#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __unix__
#include <GL/glut.h>
#endif

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	// if not called, this buffer will act as the default window buffer
	void generate();

	// generates and attaches a texture to this framebuffer
	// returns status ok flag
	bool generateTexture(GLuint width, GLuint height, bool clamp = true);

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
}

#endif//_FRAMEBUFFER_H_
