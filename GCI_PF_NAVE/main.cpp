#include "main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdParams, int cmdShow)
{
	string logMessage = "";

	_HInstance = hInstance;

	// creamos la ventana
	if (!CrearVentana(_HWnd, _HInstance, _HDC, _HGLRC, logMessage))
	{
		Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage, Fun::TipoMensaje::Error);
		return EXIT_FAILURE;
	}

	// iniciamos herramientas de glew
	if (!IniciarGlew(logMessage))
	{
		Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage, Fun::TipoMensaje::Error);
		return EXIT_FAILURE;
	}

	// iniciamos opengl
	if (!IniciarOpenGL(logMessage))
	{
		Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage, Fun::TipoMensaje::Error);
		return EXIT_FAILURE;
	}

	// mostramos ventana
	ShowWindow(_HWnd, SW_SHOW);

	// inicializamos escena
	_FirstScene = new FirstScene(&_HWnd);

	// ya cargado ponemos en primer plano
	SetForegroundWindow(_HWnd); // ponerla en primer plano
	SetFocus(_HWnd);

	// iniciamos el ciclo de la ventana/juego
	GameLoop(_HWnd, _HDC);

	// limpiamos
	delete _FirstScene;
	LimpiarVentana(_HWnd, _HInstance, _HDC, _HGLRC);

	return EXIT_SUCCESS;
}

void GameLoop(HWND hWnd, HDC hDC)
{
	MSG msg = { 0 };

	while (!false)
	{
		// usamos peekmessage en lugar de getmessage ya que este ultimo "paraliza" el programa hasta recibir un nuevo mensaje
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// si presiona escape cerramos la ventana
		if (Input::KeyIsPressed(VK_ESCAPE))
		{
			DestroyWindow(hWnd);
		}

		// si se destruye la ventana rompemos el ciclo infnito bitches
		if (msg.message == WM_QUIT)
		{
			break;
		}

		// mientras la ventana este abierta y activa en pantalla se dibujara, etc.
		if (FocusWindow)
		{
			// dibujamos la escena
			_FirstScene->Draw();

			// detectamos posicion del mouse y posicionamos en el centro
			DetectarMovimientoMouse();
			SetCursorPos(CursorPosX, CursorPosY);
			ShowCursor(FALSE);
		}

		// indicamos que ya no es el primer inicio del programa
		if (FirstInit)
			FirstInit = false;

		// al terminar de dibujar, limpiamos buffers
		SwapBuffers(hDC);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_CREATE:
	{
		break;
	}

	case WM_SETFOCUS:
	{
		FocusWindow = true;
		break;
	}

	case WM_KILLFOCUS:
	{
		// al perder el foco de la ventana, reseteamos las todas teclas a false
		// ya que el mensaje de WM_KEYUP no se volverá mandar a llamar
		// al no estar en la ventana activa
		Input::ResetKeys();
		Input::ResetMouse();
		FocusWindow = false;
		break;
	}

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		break;
	}

	case WM_KEYDOWN:
	{
		Input::Keys[wParam] = true;
		break;
	}

	case WM_KEYUP:
	{
		Input::Keys[wParam] = false;
		break;
	}

	case WM_SIZE:
	{
		OnResize(hWnd, wParam, lParam);
		break;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
		break;
	}

	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	}

	return FALSE;
}

void OnResize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	vec2 sizeWindow = Fun::GetWindowSize(hWnd);
	vec2 posWindow = Fun::GetWindowPosition(hWnd);

	WND_SIZE_WIDTH = sizeWindow.x;
	WND_SIZE_HEIGHT = sizeWindow.y;

	CursorPosX = posWindow.x + (WND_SIZE_WIDTH / 2);
	CursorPosY = posWindow.y + (WND_SIZE_HEIGHT / 2);

	glViewport(0, 0, WND_SIZE_WIDTH, WND_SIZE_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (GLfloat)WND_SIZE_WIDTH / (GLfloat)WND_SIZE_HEIGHT, SCREEN_NEAR, SCREEN_FAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}