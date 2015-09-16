/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include "../renderer/tr_local.h"
#include "gpu.h"
#include "ctr_rend.h"

//#include "kitten_bin.h"


qboolean ( * qwglSwapIntervalEXT)( int interval );
void ( * qglMultiTexCoord2fARB )( GLenum texture, float s, float t );
void ( * qglActiveTextureARB )( GLenum texture );
void ( * qglClientActiveTextureARB )( GLenum texture );


void ( * qglLockArraysEXT)( int, int);
void ( * qglUnlockArraysEXT) ( void );


void		GLimp_EndFrame( void ) {
	gpuFrameEnd();
	ctr_rend_buffer_reset();
	DBGPRINT("--------end frame -------\n\n");

	gpuFrameBegin();
}

int g_tex_id;
void 		GLimp_Init( void )
{
	gpuInit();
	glConfig.vidWidth = 400;
	glConfig.vidHeight = 240;
	glConfig.colorBits = 32;
	glConfig.depthBits = 24;
	glConfig.deviceSupportsGamma = qfalse;
	glConfig.displayFrequency = 60;
	glConfig.windowAspect = 400.0f / 240.0f;
	ri.Cvar_Set("r_picmip", "2");
	qglLockArraysEXT = glLockArraysEXT;
	qglUnlockArraysEXT = glUnlockArraysEXT;
	qglActiveTextureARB = glActiveTexture;
	qglClientActiveTextureARB = glClientActiveTexture;

	ctr_rend_init();
	// Bind the shader program
	glUseProgram(0);

#define CLEAR_COLOR 0x68B0D8FF
	gpuClearBuffers(CLEAR_COLOR);

	gpuFrameBegin();

	// Configure the first fragment shading substage to blend the texture color with
	// the vertex color (calculated by the vertex shader using a lighting algorithm)
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	GPU_SetTexEnv(0,
		GPU_TEVSOURCES(GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR), // RGB channels
		GPU_TEVSOURCES(GPU_TEXTURE0, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR), // Alpha
		GPU_TEVOPERANDS(0, 0, 0), // RGB
		GPU_TEVOPERANDS(0, 0, 0), // Alpha
		GPU_MODULATE, GPU_MODULATE, // RGB, Alpha
		0xFFFFFFFF);
	GPU_SetDummyTexEnv(1);
	GPU_SetDummyTexEnv(2);
	GPU_SetDummyTexEnv(3);
	GPU_SetDummyTexEnv(4);
	GPU_SetDummyTexEnv(5);

	// Load the texture
	//glGenTextures(1, &g_tex_id);
	//glBindTexture(0, g_tex_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, kitten_bin);
}

void		GLimp_Shutdown( void ) {
	gpuExit();
}

void		GLimp_EnableLogging( qboolean enable ) {
}

void GLimp_LogComment( char *comment ) {
}

qboolean QGL_Init( const char *dllname ) {
	return qtrue;
}

void		QGL_Shutdown( void ) {
}

void *GLimp_RendererSleep(void) {
	return 0;
}

qboolean GLimp_SpawnRenderThread(void(*function)(void)) {
	return 0;
}

void GLimp_FrontEndSleep(void) {
}

void GLimp_WakeRenderer(void *data) {
}

void GLimp_SetGamma(unsigned char red[256], unsigned char green[256], unsigned char blue[256]) {
}