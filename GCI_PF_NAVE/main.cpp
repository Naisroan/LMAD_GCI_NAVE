#include "main.h"

int main()
{
	string logMessage = "";

	// creamos la ventana del juego
	if (!CrearVentana(logMessage))
	{
		cout << logMessage << endl;
		return EXIT_FAILURE;
	}

	// iniciamos herramientas de glew
	if (!IniciarGlew(logMessage))
	{
		cout << logMessage << endl;
		return EXIT_FAILURE;
	}

	// iniciamos opengl
	if (!IniciarOpenGL(logMessage))
	{
		cout << logMessage << endl;
		return EXIT_FAILURE;
	}

	// inicializamos escena
	_FirstScene = new FirstScene(GameWindow);

	// iniciamos el ciclo de la ventana/juego
	GameLoop();

	// si se cierra la ventana legalmente (click en la x o alt+f4)...
	glfwDestroyWindow(GameWindow); // F

	// limpiamos
	delete _FirstScene;
	glfwTerminate();

	return EXIT_SUCCESS;
}

void GameLoop()
{
	while (!glfwWindowShouldClose(GameWindow))
	{
		// si es primer inicio seteamos el tiempo en 0
		/*if (FirstInit)
		{
			glfwSetTime(0.0f);
			FirstInit = false;
		}*/

		// dibujamos la escena
		_FirstScene->Draw();

		// posicionamos cursor al centro
		glfwSetCursorPos(GameWindow, 0, 0);

		// limpiamos ventana y llamamos eventos restantes
		glfwSwapBuffers(GameWindow);
		glfwPollEvents();
	}
}