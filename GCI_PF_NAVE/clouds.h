#pragma once

#ifndef _CLOUDS_
#define _CLOUDS_

#include <string>
#include "Modelo.h"

using namespace std;

class Clouds 
	: public Modelo
{

private:

	static float MovementTexture;

public:

	Clouds(string rutaPlano, string rutaTexturaNubes, string rutaVertShader, string rutaFragShader)
		: Modelo(rutaPlano, rutaTexturaNubes, "", "", rutaVertShader.c_str(), rutaFragShader.c_str())
	{
	}

	void Draw(Camera* camara, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		_Shader->use();
		_Shader->setUniform("movementTexture", Clouds::MovementTexture);
		_Shader->stopUsing();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Modelo::Draw(camara, projection, skyColor, lightPosition);
		glDisable(GL_BLEND);

		Clouds::MovementTexture += 0.0002;

		return;
	}
};

float Clouds::MovementTexture = 0.0f;

#endif