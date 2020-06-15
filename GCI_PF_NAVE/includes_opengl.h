#pragma once

#ifndef _INCLUDES_OPENGL_
#define _INCLUDES_OPENGL_

#include <glew.h>
#include <gl/GLU.h>
#include <gl/GL.h>

static bool WND_FULLSCREEN = false;
static int WND_SIZE_WIDTH = 1336;
static int WND_SIZE_HEIGHT = 728;

const double FOV = 90.0;
const double SCREEN_NEAR = 0.01;
const double SCREEN_FAR = 1000.0;
const int FPS = 60;

#endif // !_INCLUDES_OPENGL_
