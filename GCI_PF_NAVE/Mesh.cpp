#include "mesh.h"

Mesh::Mesh() { }

Mesh::~Mesh() { }

void Mesh::Draw(Shader* shader, Material* material, mat4 matrix)
{
	shader->setUniform("ambient", material->ambient);
	shader->setUniform("diffuse", material->diffuse);
	shader->setUniform("specular", material->specular);
	shader->setUniform("shininess", material->shininess);
	shader->setUniform("transparency", material->transparency);

	shader->setUniform("hasNormalMap", material->hasNormalMap());
	shader->setUniform("hasAmbientMap", material->hasAmbientMap());
	shader->setUniform("hasDiffuseMap", material->hasDiffuseMap());
	shader->setUniform("hasSpecularMap", material->hasSpecularMap());
	shader->setUniform("hasShininessMap", material->hasShininessMap());
	shader->setUniform("hasTransparencyMap", material->hasTransparencyMap());

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

	if (material->hasShininessMap()) {
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, material->shininessMap->object());
		shader->setUniform("shininessMap", 4);
	}

	if (material->hasTransparencyMap()) {
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, material->transparencyMap->object());
		shader->setUniform("transparencyMap", 5);
	}

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
}