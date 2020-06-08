#pragma once

#include <glew.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <stdio.h>
#include <algorithm>

#include "Texture.h"

using namespace ndev;
using namespace glm;
using namespace std;

class Material
{
public:
	string name;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	GLfloat shininess;
	GLfloat transparency;

	Texture* normalMap;
	Texture* ambientMap;
	Texture* diffuseMap;
	Texture* specularMap;
	Texture* shininessMap;
	Texture* transparencyMap;

	Material();
	~Material();

	GLboolean hasNormalMap();
	GLboolean hasAmbientMap();
	GLboolean hasDiffuseMap();
	GLboolean hasSpecularMap();
	GLboolean hasShininessMap();
	GLboolean hasTransparencyMap();
};

