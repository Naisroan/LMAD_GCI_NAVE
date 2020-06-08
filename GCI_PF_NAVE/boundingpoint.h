#pragma once

#include <GL/glew.h>
#include <GL/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

#include "Texture.h"

using namespace ndev;
using namespace glm;
using namespace std;

class BoundingPoint
{
public:
	vec3 initialPosition, transformedPosition;

	~BoundingPoint();
	BoundingPoint();
	BoundingPoint(vec3);
	
	GLvoid Update(mat4);
	GLboolean Collides(BoundingPoint*);
};

