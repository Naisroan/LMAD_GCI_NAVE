#pragma once
#ifndef _SCENE_
#define _SCENE_

#include <time.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <type_ptr.hpp>

#include "includes_opengl.h"
#include "input.h"
#include "funciones.h"

using namespace glm;
using namespace std;

class Scene
{

public:

	// este es el color del cielo
	vec3 skyColor = vec3(1.0, 1.0, 1.0);

private:

	const int FrameDelay = 1000 / FPS;

	double DeltaTime = 0.0f;
	double LastTime = 0.0f;

	double PreviousTime = 0.0f;
	int FrameCount = 0;

	GLFWwindow* GameWindow = 0;

protected:

	virtual void Update()
	{
	}

	virtual void Render()
	{
	}

public:

	Scene(GLFWwindow* window)
	{
		GameWindow = window;
		LastTime = GetGameSecondsElapsed();
		PreviousTime = GetGameSecondsElapsed();
	}

	void Draw()
	{
		double TimeNow = GetGameSecondsElapsed();
		DeltaTime = (float)(TimeNow - LastTime);

		// contador de fps
		// ContarFPS(TimeNow, true);

		// reinicia la ventana (pinta de un color)
		ClearWindow();	

		// logica de juego (movimientos, animaciones, etc.) 
		Update();		

		LastTime = TimeNow;

		// dibuja (buffers, shaders, etc.)
		Render();
	}

	double GetGameSecondsElapsed()
	{
		return glfwGetTime();
	}

	double GetGameMillisElapsed()
	{
		return glfwGetTime() * 1000;
	}

	float GetDeltaTime()
	{
		return DeltaTime;
	}

	mat4 GetProjection()
	{
		return perspective(radians((float)FOV), ((float)WND_SIZE_WIDTH / (float)WND_SIZE_HEIGHT), (float)SCREEN_NEAR, (float)SCREEN_FAR);
	}

	GLFWwindow* GetGameWindow()
	{
		return GameWindow;
	}

	void SetSkyColor(float skyTime, bool toNight)
	{
		vec3 color = vec3(skyColor);

		if (toNight)
		{
			if (skyTime > 1.0f && skyTime < 2.0f) // si es mayor o igual a 2 quiere que decir que apenas es de dia y va hacia tarde
			{
				// obteniendo un color naranja gradualmente (.6, .5, .5)
				color.x = color.x - 0.0005f < 0.6f ? 0.6f : color.x - 0.0005f;
				color.y = color.y - 0.0006f < 0.5f ? 0.5f : color.y - 0.0006f;
				color.z = color.z - 0.0006f < 0.5f ? 0.5f : color.z - 0.0006f;
			}

			if (skyTime <= 1.0f) // si es menor o igual a 1 quiere que decir que ya es de tarde y va hacia noche
			{
				// obteniendo un color azul gradualmente (.1, .1, .2)
				color.x = color.x - 0.0006f < 0.1f ? 0.1f : color.x - 0.0006f;

				if (color.x <= 0.5f) // el rojo se baja hasta el 0.5 y empiezan los demas para que sea gradual
				{
					color.y = color.y - 0.0006f < 0.1f ? 0.1f : color.y - 0.0006f;
					color.z = color.z - 0.0006f < 0.2f ? 0.2f : color.z - 0.0006f;
				}
			}
		}
		else
		{
			if (skyTime > 0.0f && skyTime < 1.0f) // si es menor que 1 quiere decir que es de noche y va hacia tarde (o mañana/madrugada)
			{
				// obteniendo un color naranja gradualmente (.6, .5, .5)
				color.x = color.x + 0.0006f > 0.6f ? 0.6f : color.x + 0.0006f;
				color.y = color.y + 0.0005f > 0.5f ? 0.5f : color.y + 0.0005f;

				if (color.y >= 0.2f)
				{
					color.z = color.z + 0.0005f > 0.5f ? 0.5f : color.z + 0.0005f;
				}
			}

			if (skyTime >= 1.0f) // si es mayor o igual a 1 quiere decir que es de mañana/madrugada y va hacia dia
			{
				// obteniendo un color blanco gradualmente (1, 1, 1)
				color.y = color.y + 0.0006f > 1.0f ? 1.0f : color.y + 0.0006f;
				color.z = color.z + 0.0006f > 1.0f ? 1.0f : color.z + 0.0006f;

				if (color.y >= 0.6f && color.z >= 0.6f)
				{
					color.x = color.x + 0.0006f > 1.0f ? 1.0f : color.x + 0.0006f;
				}
			}
		}

		skyColor = color;
	}

	vec3 GetSkyColor()
	{
		return skyColor;
	}

private:

	void ContarFPS(float timeNow, bool display = false)
	{
		FrameCount++;

		if (timeNow - PreviousTime >= 1.0f)
		{
			if (display)
				cout << "FPS: " << FrameCount << endl;

			FrameCount = 0;
			PreviousTime = timeNow;
		}
	}

	void ClearWindow()
	{
		glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}
};

#endif // !_SCENE_
