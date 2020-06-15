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

// constantes
const char* NOMBRE_CLASE_VENTANA = "WND_NAVE_CLASS";

// variables globales
static HINSTANCE _HInstance;
static HWND _HWnd;
static HDC _HDC;
static HGLRC _HGLRC;
static float CursorPosX;
static float CursorPosY;

bool FirstInit = true;
bool FocusWindow = false;

// escenas
FirstScene* _FirstScene;

// prototipos
bool CrearVentana(HWND& hWnd, HINSTANCE& hInstance, HDC& hDC, HGLRC& hGLRC, string& logMessage);
bool IniciarOpenGL(string& logMessage);
bool IniciarGlew(string& logMessage);
void GameLoop(HWND hWnd, HDC hDC);
void LimpiarVentana(HWND& hWnd, HINSTANCE& hInstance, HDC& hDC, HGLRC& hGLRC);
void DetectarMovimientoMouse();
void OnResize(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// definiciones
bool CrearVentana(HWND& hWnd, HINSTANCE& hInstance, HDC& hDC, HGLRC& hGLRC, string& logMessage)
{
	WNDCLASSEX wndClass;

	// creacion de la clase, estructura de la ventana
	wndClass.hInstance = hInstance;
	wndClass.lpszClassName = NOMBRE_CLASE_VENTANA;
	wndClass.lpfnWndProc = WndProc;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // dibuja de nuevo al cambiar el ancho y/o altura
	wndClass.cbSize = sizeof(WNDCLASSEX);

	// apariencia
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszMenuName = NULL;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	// registro de la clase
	if (!RegisterClassEx(&wndClass))
	{
		logMessage = MSG_ERROR_REGISTRO_VENTANA;
		return false;
	}

	// calculamos algunos valores para la posicion de la ventana
	vec2 screenResSize = Fun::GetDesktopWindowSize();

	int wndPosX = WND_FULLSCREEN ? 0 : screenResSize.x / 2 - WND_SIZE_WIDTH / 2;
	int wndPosY = WND_FULLSCREEN ? 0 : screenResSize.y / 2 - WND_SIZE_HEIGHT / 2;
	int wndWidthSize = WND_FULLSCREEN ? screenResSize.x : WND_SIZE_WIDTH;
	int wndHeightSize = WND_FULLSCREEN ? screenResSize.y : WND_SIZE_HEIGHT;

	// establecemos la posicion del cursor por default
	CursorPosX = wndPosX + (wndWidthSize / 2);
	CursorPosY = wndPosY + (wndHeightSize / 2);

	// establecemos pantalla completa si asi se indica
	Fun::EstablecerModoPantallaCompleta(WND_FULLSCREEN);

	// creacion de la ventana
	hWnd = CreateWindowEx(0,
		NOMBRE_CLASE_VENTANA,
		APP_NAME,
		WND_FULLSCREEN ? WS_POPUP : WS_OVERLAPPEDWINDOW,
		wndPosX,
		wndPosY,
		wndWidthSize,
		wndHeightSize,
		0,
		NULL,
		hInstance,
		NULL);

	if (hWnd == NULL)
	{
		logMessage = MSG_ERROR_CREACION_VENTANA;
		return false;
	}

	// obtenemos el contexto del dispositivo
	hDC = GetDC(hWnd);

	if (hDC == NULL)
	{
		logMessage = MSG_ERROR_CONTEXTO_DISPOSITIVO;
		return false;
	}

	// creamos el formato de pixeles para el dipositivo de contexto de la ventana
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	// buscamos un formato que coincida con nuestra configuracion
	int pixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (pixelFormat == 0)
	{
		logMessage = MSG_ERROR_PIXELFORMAT;
		return false;
	}

	// si se encontro, lo establecemos al contexto del dispositivo
	if (!SetPixelFormat(hDC, pixelFormat, &pfd))
	{
		logMessage = MSG_ERROR_ESTABLECER_PIXELFORMAT;
		return false;
	}

	// creamos el contexto para OpenGL (donde se representara todos los pixeles dibujados)
	hGLRC = wglCreateContext(hDC);

	if (hGLRC == NULL)
	{
		logMessage = MSG_ERROR_CONTEXTO_REPRESENTACION;
		return false;
	}

	// ya creado el contexto de representacion hay que activarlo
	if (!wglMakeCurrent(hDC, hGLRC))
	{
		logMessage = MSG_ERROR_CONTEXTO_REPRESENTACION_ACTIVAR;
		return false;
	}

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

void LimpiarVentana(HWND& hWnd, HINSTANCE& hInstance, HDC& hDC, HGLRC& hGLRC)
{
	ChangeDisplaySettings(NULL, 0);
	ShowCursor(TRUE);
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hGLRC);
	ReleaseDC(hWnd, hDC);
	UnregisterClass(NOMBRE_CLASE_VENTANA, hInstance);
}

void DetectarMovimientoMouse()
{
	if (FirstInit)
		return;

	glm::vec2 cursorPos = Fun::GetCursorPosition();

	float xPos = cursorPos.x;
	float yPos = cursorPos.y;

	// con los offset detectamos si hubo un movmiento, ya sea vertical o horizontal
	float xOffset = xPos - CursorPosX;
	float yOffset = yPos - CursorPosY;

	Input::MouseX = xOffset;
	Input::MouseY = yOffset;
}

#endif // !_MAIN_