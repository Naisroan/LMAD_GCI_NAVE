#pragma once

#ifndef _INPUT_
#define _INPUT_

#include <Windows.h>

#define CANT_TECLAS 256

class Input 
{

public:

	enum class CustomKey
	{
		W = 87,
		D = 68,
		S = 83,
		A = 65,
		ESC = 27,
		SPACE = 32,
		L_SHIFT = VK_SHIFT,
		I = 73,
		L = 76,
		K = 75,
		J = 74,
		U = 85,
		O = 79,
		UP = VK_UP,
		DOWN = VK_DOWN,
		LEFT = VK_LEFT,
		RIGHT = VK_RIGHT,
	};

	static bool Keys[CANT_TECLAS];
	static float MouseX;
	static float MouseY;

	static bool KeyIsPressed(int tecla)
	{
		return Keys[tecla];
	}

	static bool KeyIsPressed(CustomKey tecla)
	{
		return Keys[(int)tecla];
	}

	static void ResetKeys()
	{
		memset(&Input::Keys, false, sizeof(Keys));
	}

	static float GetMouseX()
	{
		return MouseX;
	}

	static float GetMouseY()
	{
		return MouseY;
	}

	static void ResetMouse()
	{
		MouseX = 0;
		MouseY = 0;
	}
};

bool Input::Keys[CANT_TECLAS] = { false };
float Input::MouseX = 0;
float Input::MouseY = 0;

#endif // !_INPUT_
