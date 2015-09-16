#include "gl.h"
#include <stdio.h>

#if 0
#define DBGPRINT printf
#else
#define DBGPRINT(...) /**/  
#endif


void glTexEnvf(GLenum val0, GLenum val1, GLenum val2) {
	DBGPRINT("\n");
}


void glBlendFunc(GLenum a, GLenum b) {
	DBGPRINT("\n");
}


void glDepthMask(GLenum a) {
	DBGPRINT("\n");
}


void glPolygonMode(GLenum a, GLenum b) {
	DBGPRINT("\n");
}


void glCullFace(GLenum a) {
	DBGPRINT("\n");
}


void glDepthFunc(GLenum a) {
	DBGPRINT("\n");
}


void glDepthRange(GLfloat a, GLfloat b) {
	DBGPRINT("\n");
}


void glTexParameterf(GLenum a, GLenum b, GLfloat c) {
	DBGPRINT("\n");
}


void glTexParameterfv(GLenum a, GLenum b, GLfloat *c) {
	DBGPRINT("\n");
}


void glReadPixels(GLenum a, GLenum b, GLenum c, GLenum d, GLenum e, GLenum f, void *p) {
	DBGPRINT("\n");
}


void glDrawBuffer(GLenum a) {
	DBGPRINT("\n");
}


void glClear(GLenum a) {
	DBGPRINT("\n");
}


void glAlphaFunc(GLenum a, GLfloat b) {
	DBGPRINT("\n");
}


void glClearColor(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	DBGPRINT("\n");
}


void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
	DBGPRINT("\n");
}


void glFinish() {
	DBGPRINT("\n");
}


void glClipPlane(GLenum plane, const GLdouble *equation) {
	DBGPRINT("\n");
}


void glClearStencil(GLenum a) {
	DBGPRINT("\n");
}


GLenum glGetError() {
	DBGPRINT("\n");
}


void glShadeModel(GLenum a) {
	DBGPRINT("\n");
}


void glColor4f(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
	DBGPRINT("\n");
}


void glGetIntegerv(GLenum pname, GLint *params)
{
	switch (pname) {
	case GL_MAX_TEXTURE_SIZE:
		*params = 128;
		break;
	case GL_MAX_ACTIVE_TEXTURES:
		*params = 3;
		break;
	default:
		DBGPRINT("glGetIntegerv: %d\n", pname);
		waitforit("glGetIntegerv");
	}
}


void glColor4ubv(const GLubyte *v) {
	DBGPRINT("\n");
}


void glTexCoord2fv(GLfloat *a) {
	DBGPRINT("\n");
}


void glPolygonOffset(GLfloat factor, GLfloat units) {
	DBGPRINT("\n");
}


void glArrayElement(GLint i) {
	DBGPRINT("\n");
}

void glClearDepth(GLclampd depth) {
}

void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
}

void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass) {
}

void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
}


void glLineWidth(GLfloat width) {
}

void glStencilMask(GLuint mask) {
}

void glCallList(GLuint list) {
}
