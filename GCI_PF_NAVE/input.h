#pragma once

#ifndef _INPUT_
#define _INPUT_

#define CANT_TECLAS 256

class Input 
{

public:

	enum class CustomKey
	{
		W = 87,
		D = 68,
		S = 83,
		A = 65
	};

	static bool Keys[CANT_TECLAS];
	static int MouseX;
	static int MouseY;

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

	static int GetMouseX()
	{
		return MouseX;
	}

	static int GetMouseY()
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
int Input::MouseX = 0;
int Input::MouseY = 0;

#endif // !_INPUT_
