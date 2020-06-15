#pragma once

#include "quad.h"
#include "Camera.h"
#include "Geometria.h"

class Billboard
	: public Geometria
{
public:

	vec2 size;

	Billboard(Shader* shader, string rutaTextura, vec2 size);
	~Billboard();

	void Draw(Camera* camera, mat4 projection, vec3 skyColor, vec3 lightPosition, bool toCenter = false);

private:

	Texture* textura;

	Quad* gQuad;
	Shader* gProgram;
};