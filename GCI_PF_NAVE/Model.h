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

	static vector<Material> CargarMTL(string, string);
	static vector<Group> CargarModelo(Shader*, string, string, string&);
	static GLvoid CalcularTangentes(vector<Mesh::Vertex>&, vector <GLuint>&);
	static GLvoid GenerarVAO(Shader*, Group&, vector<Mesh::Vertex>&, vector<GLuint>&);

public:

	vector<Group> groups;
	vector<Material> materials;

	~Model();

	static Model* CargarOBJ(Shader*, string, string);

	GLvoid Draw(Shader* shader, mat4 view, mat4 projection);

};