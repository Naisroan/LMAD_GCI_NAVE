#pragma once
#ifndef _SCENE_
#define _SCENE_

#include <time.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes_opengl.h"
#include "input.h"
#include "funciones.h"

using namespace glm;
using namespace std;

class Scene
{

private:

	const int FrameDelay = 1000 / FPS;

	double DeltaTime = 0.0f;
	double LastTime = 0.0f;

	double PreviousTime = 0.0f;
	int FrameCount = 0;

	GLFWwindow* GameWindow = 0;

	vec3 ClearColor = vec3(1.0, 0.9, 0.9);

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

	void SetClearColor(vec3 color)
	{
		color.x = color.x <= 0.009f ? 0.009f : color.x >= 1.0f ? 1.0f : color.x;
		color.y = color.y <= 0.008f ? 0.008f : color.y >= 0.9f ? 0.9f : color.y;
		color.z = color.z <= 0.008f ? 0.008f : color.z >= 0.9f ? 0.9f : color.z;

		ClearColor = color;
	}

	vec3 GetClearColor()
	{
		return ClearColor;
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
		glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}
};

#endif // !_SCENE_
