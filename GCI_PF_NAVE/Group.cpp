#include "group.h"

Group::Group() { }

Group::~Group() { }

void Group::Draw(Shader* program, vector<Material>& materials, mat4 matrix, mat4 view, mat4 projection)
{
	program->use();
	program->setUniform("model", matrix);
	program->setUniform("projection", projection);
	program->setUniform("camera", view);

	glBindVertexArray(VAO);

	for (auto& mesh : meshes) 
		mesh.Draw(program, &materials.at(mesh.materialIndex), matrix);

	glBindVertexArray(0);
	program->stopUsing();
}