#pragma once

#include <vector>
#include "mesh.h"
#include "material.h"

using namespace std;

class Group
{
public:
	string name;
	vector<Mesh> meshes;

	Collider collider;
	GLuint VAO;

	Group();
	~Group();

	GLvoid Draw(Shader*, vector<Material>&, mat4, mat4 view, mat4 projection);
	GLvoid Update(mat4);

	GLboolean Collides(Group*);
};

