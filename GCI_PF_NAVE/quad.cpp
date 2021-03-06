#include "quad.h"

Quad::~Quad() { }

Quad::Quad(Shader* _shader)
{
	gProgram = _shader;

	GLfloat vertexData[] = {
		0.0, 0.0, 0.0f,		0.0, 0.0,
		0.0, 1.0, 0.0f,		0.0, 1.0,
		1.0, 0.0, 0.0f,		1.0, 0.0,
		1.0, 1.0, 0.0f,		1.0, 1.0,
	};

	GLuint indexData[] = {
		0, 1, 2,
		3, 2, 1
	};

	GLuint gVBO, gIBO;

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(gProgram->attrib("vertPosition"));
	glVertexAttribPointer(gProgram->attrib("vertPosition"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLvoid Quad::Draw(GLuint texture, mat4 modelMatrix, mat4 view, mat4 projection)
{
	gProgram->use();
	gProgram->setUniform("model", modelMatrix);
	gProgram->setUniform("camera", view);
	gProgram->setUniform("projection", projection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	gProgram->setUniform("map", 0);

	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	gProgram->stopUsing();
}