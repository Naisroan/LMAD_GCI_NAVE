#include "mesh.h"

Mesh::Mesh() { }

Mesh::~Mesh() { }

void Mesh::Draw(Shader* shader, Material* material, mat4 matrix)
{
	shader->setUniform("ambient", material->ambient);
	shader->setUniform("diffuse", material->diffuse);
	shader->setUniform("specular", material->specular);

	shader->setUniform("tieneNormalMap", material->hasNormalMap());
	shader->setUniform("tieneAmbientMap", material->hasAmbientMap());
	shader->setUniform("tieneDiffuseMap", material->hasDiffuseMap());
	shader->setUniform("tieneSpecularMap", material->hasSpecularMap());

	if (material->hasNormalMap()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->normalMap->object());
		shader->setUniform("normalMap", 0);
	}

	if (material->hasAmbientMap()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->ambientMap->object());
		shader->setUniform("ambientMap", 1);
	}

	if (material->hasDiffuseMap()) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->diffuseMap->object());
		shader->setUniform("diffuseMap", 2);
	}

	if (material->hasSpecularMap()) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material->specularMap->object());
		shader->setUniform("specularMap", 3);
	}

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}