#pragma once

#include <GL/glew.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>

#include "Shader.h"
#include "Texture.h"

using namespace std;
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

