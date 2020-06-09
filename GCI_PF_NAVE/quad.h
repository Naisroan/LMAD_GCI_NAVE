#pragma once

#include <glew.h>
#include <GL/GLU.h>
#include <glfw3.h>
#include <glm.hpp>
#include <matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

using namespace glm;
using namespace ndev;

class Quad
{
public:
	Quad(Shader*);
	~Quad();

	GLvoid Draw(GLuint, mat4, mat4, mat4);
private:
	Shader* gProgram;
	GLuint gVAO;
};

