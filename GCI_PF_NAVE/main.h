#pragma once

#pragma once

#ifndef _MAIN_
#define _MAIN_

// librerias del sistema
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <exception>

// librerias de terceros en sistema
#include <glm.hpp>
#include "includes_opengl.h"

// librerias de terceros propias
#include "constantes.h"
#include "funciones.h"
#include "input.h"
#include "FirstScene.h"

// uso de namespaces
using namespace std;
using namespace glm;

// variables globales
GLFWwindow* GameWindow;

static float CursorPosX;
static float CursorPosY;
bool FirstInit = true;

// escenas
FirstScene* _FirstScene;

// prototipos
void GameLoop();

// definiciones
void OnResize()
{
	int width, size;
	int xPos, yPos;

	glfwGetWindowSize(GameWindow, &width, &size);
	glfwGetWindowPos(GameWindow, &xPos, &yPos);

	WND_SIZE_WIDTH = width;
	WND_SIZE_HEIGHT = size;

	CursorPosX = xPos + (WND_SIZE_WIDTH / 2);
	CursorPosY = yPos + (WND_SIZE_HEIGHT / 2);

	glViewport(0, 0, WND_SIZE_WIDTH, WND_SIZE_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (GLfloat)WND_SIZE_WIDTH / (GLfloat)WND_SIZE_HEIGHT, SCREEN_NEAR, SCREEN_FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool CrearVentana(string& logMessage)
{
	// verificamos que inicia el glfw
	if (!glfwInit())
	{
		logMessage = MSG_ERROR_GLFW;
		return false;
	}

	// establecemos compatibilidad con opengl (no se exactamente para que funcionen)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// calculamos algunos valores para la posicion de la ventana
	vec2 screenResSize = Fun::GetDesktopWindowSize();

	int wndPosX = WND_FULLSCREEN ? 0 : screenResSize.x / 2 - WND_SIZE_WIDTH / 2;
	int wndPosY = WND_FULLSCREEN ? 0 : screenResSize.y / 2 - WND_SIZE_HEIGHT / 2;
	int wndWidthSize = WND_FULLSCREEN ? screenResSize.x : WND_SIZE_WIDTH;
	int wndHeightSize = WND_FULLSCREEN ? screenResSize.y : WND_SIZE_HEIGHT;
	
	// establecemos la posicion del cursor por default
	CursorPosX = wndPosX + (wndWidthSize / 2);
	CursorPosY = wndPosY + (wndHeightSize / 2);

	// creacion de la ventana
	if (WND_FULLSCREEN)
	{
		// establecemos pantalla completa si asi se indica
		GameWindow = glfwCreateWindow(WND_SIZE_WIDTH, WND_SIZE_HEIGHT, APP_NAME, glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		GameWindow = glfwCreateWindow(WND_SIZE_WIDTH, WND_SIZE_HEIGHT, APP_NAME, NULL, NULL);
	}

	if (!GameWindow)
	{
		logMessage = MSG_ERROR_CREACION_VENTANA;
		return false;
	}

	// configuramos la ventana

	// > posicion de la ventana
	glfwSetWindowPos(GameWindow, wndPosX, wndPosY);

	// > le indicamos que no se puede reajustar su tamaño
	glfwSetWindowAttrib(GameWindow, GLFW_RESIZABLE, GLFW_FALSE);	

	// > le indicamos que no tenga bordes porque se ven feos xd
	glfwSetWindowAttrib(GameWindow, GLFW_DECORATED, GLFW_FALSE);		

	// > posicion de la ventana
	glfwSetInputMode(GameWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	

	// > establecemos la posicion del cursor
	glfwSetCursorPos(GameWindow, 0, 0);				

	// > establecemos que OpenGL trabajara en la nueva ventana creada
	glfwMakeContextCurrent(GameWindow);

	// > establecemos evento si es que se cambia de tamaño
	// glfwSetWindowSizeCallback(GameWindow, OnResize);
	OnResize();
	
	return true;
}

bool IniciarOpenGL(string& logMessage)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	return true;
}

bool IniciarGlew(string& logMessage)
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		logMessage = MSG_ERROR_GLEW;
		return false;
	}

	return true;
}

#endif // !_MAIN_