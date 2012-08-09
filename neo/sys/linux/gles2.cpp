/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Krzysztof Klinikowski <kkszysiu@gmail.com>
Copyright (C) 2012 Havlena Petr <havlenapetr@gmail.com>

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "../../idlib/precompiled.h"
#include "../../renderer/tr_local.h"
#include "local.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

idCVar sys_videoRam("sys_videoRam", "0", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_INTEGER,
        "Texture memory on the video card (in megabytes) - 0: autodetect", 0, 512);

static ANativeWindow* window = NULL;
static EGLDisplay eglDisplay = EGL_NO_DISPLAY;
static EGLContext eglContext = EGL_NO_CONTEXT;
static EGLSurface eglSurface = EGL_NO_SURFACE;
static EGLConfig eglConfig = NULL;
static EGLint eglNumConfig = NULL;

// backup gamma ramp
static int save_rampsize = 0;
static unsigned short *save_red, *save_green, *save_blue;

static
void GLimp_AndroidSetResolution(int32_t width, int32_t height) {
    cvarSystem->SetCVarBool("r_fullscreen",  true);
    cvarSystem->SetCVarInteger("r_mode", -1);

    cvarSystem->SetCVarInteger("r_customWidth", width);
    cvarSystem->SetCVarInteger("r_customHeight", height);

    float r = (float) width / (float) height;
    if (r > 1.7f) {
        cvarSystem->SetCVarInteger("r_aspectRatio", 1);    // 16:9
    } else if (r > 1.55f) {
        cvarSystem->SetCVarInteger("r_aspectRatio", 2);    // 16:10
    } else {
        cvarSystem->SetCVarInteger("r_aspectRatio", 0);    // 4:3
    }

    Sys_Printf("r_mode(%i), r_customWidth(%i), r_customHeight(%i)",
            -1, width, height);
}

void GLimp_AndroidInit(ANativeWindow* win) {
    assert(win);
    ANativeWindow_acquire(win);
    window = win;
}

void GLimp_AndroidQuit() {
    if(window) {
        ANativeWindow_release(window);
    }
}

void GLimp_WakeBackEnd(void *a)
{
	common->DPrintf("GLimp_WakeBackEnd stub\n");
}

void GLimp_EnableLogging(bool log)
{
	//common->DPrintf("GLimp_EnableLogging stub\n");
}

void GLimp_FrontEndSleep()
{
	common->DPrintf("GLimp_FrontEndSleep stub\n");
}

void *GLimp_BackEndSleep()
{
	common->DPrintf("GLimp_BackEndSleep stub\n");
	return 0;
}

bool GLimp_SpawnRenderThread(void (*a)())
{
	common->DPrintf("GLimp_SpawnRenderThread stub\n");
	return false;
}

void GLimp_ActivateContext()
{
    if(eglSurface == EGL_NO_SURFACE && eglContext == EGL_NO_CONTEXT) {
        Sys_Printf("Fetching EGL surface and context");

        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, window, NULL);

        EGLint ctxAttrib[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
        };
        eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxAttrib);

        if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
            Sys_Error("Unable to eglMakeCurrent");
            return;
        }
    }

    int32_t screenWidth, screenHeight;
    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &screenWidth);
    eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &screenHeight);
    GLimp_AndroidSetResolution(screenWidth, screenHeight);

    Sys_Printf("Surface parms: width(%i), height(%i)",
            screenWidth, screenHeight);
}

void GLimp_DeactivateContext()
{
    if (eglDisplay == EGL_NO_DISPLAY) {
        return;
    }

    if (eglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(eglDisplay, eglContext);
        eglContext = EGL_NO_CONTEXT;
    }
}

/*
=================
GLimp_SaveGamma

save and restore the original gamma of the system
=================
*/
void GLimp_SaveGamma()
{
	if (save_rampsize) {
		return;
	}
}

/*
=================
GLimp_RestoreGamma

save and restore the original gamma of the system
=================
*/
void GLimp_RestoreGamma()
{
}

/*
=================
GLimp_SetGamma

gamma ramp is generated by the renderer from r_gamma and r_brightness for 256 elements
the size of the gamma ramp can not be changed on X (I need to confirm this)
=================
*/
void GLimp_SetGamma(unsigned short red[256], unsigned short green[256], unsigned short blue[256])
{
}

void GLimp_Shutdown()
{
    GLimp_DeactivateContext();

    if (eglDisplay == EGL_NO_DISPLAY) {
        return;
    }

    if (eglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(eglDisplay, eglSurface);
    }
    if(eglDisplay != EGL_NO_DISPLAY) {
        eglTerminate(eglDisplay);
    }

    eglDisplay = EGL_NO_DISPLAY;
    eglContext = EGL_NO_CONTEXT;
    eglSurface = EGL_NO_SURFACE;
}

void GLimp_SwapBuffers()
{
	assert(eglDisplay && eglSurface);
	eglSwapBuffers(eglDisplay, eglSurface);
}

bool GLimp_OpenDisplay(void)
{
	if (cvarSystem->GetCVarInteger("net_serverDedicated") == 1) {
		common->DPrintf("not opening the display: dedicated server\n");
		return false;
	}

	common->Printf("Setup Android SurfaceFlinger display connection\n");

	if (!(eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY))) {
		common->Printf("Couldn't open the EGL display\n");
		return false;
	}

	if (!eglInitialize(eglDisplay, NULL, NULL)) {
		common->Printf("Couldn't initialize EGL\n");
		return false;
	}

	return true;
}

/*
===============
GLES_Init
===============
*/

int GLES_Init(glimpParms_t a)
{
	EGLint attrib[] = {
		EGL_RED_SIZE, 8,	//  1,  2
		EGL_GREEN_SIZE, 8,	//  3,  4
		EGL_BLUE_SIZE, 8,	//  5,  6
		EGL_ALPHA_SIZE, 8,	//  7,  8
		EGL_DEPTH_SIZE, 24,	//  9, 10
		EGL_STENCIL_SIZE, 8,	// 11, 12
		EGL_BUFFER_SIZE, 24,	// 13, 14
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,	// 15, 16
		EGL_NONE,	// 17
	};
	// these match in the array
#define ATTR_RED_IDX	1
#define ATTR_GREEN_IDX	3
#define ATTR_BLUE_IDX	5
#define ATTR_ALPHA_IDX	7
#define ATTR_DEPTH_IDX	9
#define ATTR_STENCIL_IDX	11
#define ATTR_BUFFER_SIZE_IDX	13

	unsigned long mask;
	int colorbits, depthbits, stencilbits;
	int tcolorbits, tdepthbits, tstencilbits;
	int actualWidth, actualHeight;
	int i;
	const char *glstring;
    EGLint format;

    if(!window) {
        Sys_Error("Can't init, because I haven't window!");
        return false;
    }

	if (!GLimp_OpenDisplay()) {
		return false;
	}

	common->Printf("Initializing OpenGL display\n");

	actualWidth = glConfig.vidWidth;
	actualHeight = glConfig.vidHeight;

	// color, depth and stencil
	colorbits = 24;
	depthbits = 24;
	stencilbits = 8;

	for (i = 0; i < 16; i++) {
		// 0 - default
		// 1 - minus colorbits
		// 2 - minus depthbits
		// 3 - minus stencil
		if ((i % 4) == 0 && i) {
			// one pass, reduce
			switch (i / 4) {
				case 2:

					if (colorbits == 24)
						colorbits = 16;

					break;
				case 1:

					if (depthbits == 24)
						depthbits = 16;
					else if (depthbits == 16)
						depthbits = 8;

				case 3:

					if (stencilbits == 24)
						stencilbits = 16;
					else if (stencilbits == 16)
						stencilbits = 8;
			}
		}

		tcolorbits = colorbits;
		tdepthbits = depthbits;
		tstencilbits = stencilbits;

		if ((i % 4) == 3) {		// reduce colorbits
			if (tcolorbits == 24)
				tcolorbits = 16;
		}

		if ((i % 4) == 2) {		// reduce depthbits
			if (tdepthbits == 24)
				tdepthbits = 16;
			else if (tdepthbits == 16)
				tdepthbits = 8;
		}

		if ((i % 4) == 1) {		// reduce stencilbits
			if (tstencilbits == 24)
				tstencilbits = 16;
			else if (tstencilbits == 16)
				tstencilbits = 8;
			else
				tstencilbits = 0;
		}

		if (tcolorbits == 24) {
			attrib[ATTR_RED_IDX] = 8;
			attrib[ATTR_GREEN_IDX] = 8;
			attrib[ATTR_BLUE_IDX] = 8;
			attrib[ATTR_BUFFER_SIZE_IDX] = 24;
		} else {
			// must be 16 bit
			attrib[ATTR_RED_IDX] = 4;
			attrib[ATTR_GREEN_IDX] = 4;
			attrib[ATTR_BLUE_IDX] = 4;
			attrib[ATTR_BUFFER_SIZE_IDX] = 16;
		}

		attrib[ATTR_DEPTH_IDX] = tdepthbits;	// default to 24 depth
		attrib[ATTR_STENCIL_IDX] = tstencilbits;

		if (!eglChooseConfig(eglDisplay, attrib, &eglConfig, 1, &eglNumConfig)) {
			common->Printf("Couldn't get a EGL config 0x%04x\n", eglGetError());
			continue;
		}

		common->Printf("Using %d/%d/%d Color bits, %d Alpha bits, %d depth, %d stencil display.\n",
		               attrib[ATTR_RED_IDX], attrib[ATTR_GREEN_IDX],
		               attrib[ATTR_BLUE_IDX], attrib[ATTR_ALPHA_IDX],
		               attrib[ATTR_DEPTH_IDX],
		               attrib[ATTR_STENCIL_IDX]);

		glConfig.colorBits = tcolorbits;
		glConfig.depthBits = tdepthbits;
		glConfig.stencilBits = tstencilbits;
		break;
	}

    eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(window, 0, 0, format);

	if (!eglNumConfig) {
		common->Printf("No acceptable EGL configurations found!\n");
		return false;
	}

    GLimp_ActivateContext();

	glstring = (const char *) glGetString(GL_RENDERER);
	common->Printf("GL_RENDERER: %s\n", glstring);

	glstring = (const char *) glGetString(GL_EXTENSIONS);
	common->Printf("GL_EXTENSIONS: %s\n", glstring);

	glConfig.isFullscreen = true;

	if (glConfig.isFullscreen) {
		Sys_GrabMouseCursor(true);
	}

	return true;
}

/*
===================
GLimp_Init

This is the platform specific OpenGL initialization function.  It
is responsible for loading OpenGL, initializing it,
creating a window of the appropriate size, doing
fullscreen manipulations, etc.  Its overall responsibility is
to make sure that a functional OpenGL subsystem is operating
when it returns to the ref.

If there is any failure, the renderer will revert back to safe
parameters and try again.
===================
*/
bool GLimp_Init(glimpParms_t a)
{

	if (!GLimp_OpenDisplay()) {
		return false;
	}

	if (!GLES_Init(a)) {
		return false;
	}

	return true;
}

/*
===================
GLimp_SetScreenParms
===================
*/
bool GLimp_SetScreenParms(glimpParms_t parms)
{
	return true;
}

/*
================
Sys_GetVideoRam
returns in megabytes
open your own display connection for the query and close it
using the one shared with GLimp_Init is not stable
================
*/
int Sys_GetVideoRam(void)
{
	static int run_once = 0;
	int major, minor, value;

	if (run_once) {
		return run_once;
	}

	if (sys_videoRam.GetInteger()) {
		run_once = sys_videoRam.GetInteger();
		return sys_videoRam.GetInteger();
	}

	// try a few strategies to guess the amount of video ram
	common->Printf("guessing video ram ( use +set sys_videoRam to force ) ..\n");

	if (!GLimp_OpenDisplay()) {
		run_once = 64;
		return run_once;
	}

	if (int sysRam = Sys_GetSystemRam()) {
        run_once = sysRam/2; // Most SoCs has shared ram with vram...
        return run_once;
	}

	common->Printf("guess failed, return default low-end VRAM setting ( 64MB VRAM )\n");
	run_once = 64;
	return run_once;
}
