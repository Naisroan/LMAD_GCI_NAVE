#pragma once

#include <fstream>
#include "group.h"
#include <Windows.h>
#include "Geometria.h"

class Model
	: public Geometria
{
private:
	Model();

	static vector <Material> LoadMtl(string, string);
	static vector <Group> LoadObj(Shader*, string, string, vec3&, vec3&, string&);
	static GLvoid ComputeTangentSpace(vector<Mesh::Vertex>&, vector <GLuint>&);
	static GLvoid CreateBuffers(Shader*, Group&, vector<Mesh::Vertex>&, vector<GLuint>&);
	static Collider CalculateCollider(vec3&, vec3&);

public:
	vector<Group> groups;
	vector<Material> materials;

	Collider collider;

	~Model();

	static Model* ObjToModel(Shader*, string, string);

	GLvoid Draw(Shader*, mat4 view, mat4 projection);
	GLvoid Update(mat4);

	GLboolean Collides(Model*);
};