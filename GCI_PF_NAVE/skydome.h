#pragma once

#ifndef _SKYDOME_
#define _SKYDOME_

#include <string>
#include "Geometria.h"
#include "Maya.h"
#include "Picture.h"
#include "Camera.h"

using namespace std;

class Skydome
	: public Geometria, Maya
{

private:

	unsigned int TextureID;
	const int DEFAULT_STACKS = 32;
	const int DEFAULT_SLICES = 32;
	const int DEFAULT_RADIO = 250;

public:

	Skydome(string rutaTextura, string rutaVertShader, string rutaFragShader)
		: Geometria(rutaVertShader.c_str(), rutaFragShader.c_str())
	{
		string logMessage = "";
		Maya mayaEsfera = Esfera(DEFAULT_STACKS, DEFAULT_SLICES, DEFAULT_RADIO, 0.5f, 1.0f);

		if (!CargarTextura(rutaTextura, logMessage))
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage);
			throw exception(logMessage.c_str());
			PostQuitMessage(0);
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, CantidadVertices * sizeof(Vertices), mayaEsfera._Maya, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);  // Vertex position.
		glEnableVertexAttribArray(1);  // Normals.
		glEnableVertexAttribArray(2);  // Texture coordinates.

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertices), 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (3 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (6 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (8 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertices), (unsigned char*)NULL + (11 * sizeof(float)));

		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, CantidadIndices * sizeof(unsigned int), mayaEsfera.Indices, GL_STATIC_DRAW);

		delete[] mayaEsfera._Maya;
		mayaEsfera._Maya = 0;

		delete[] mayaEsfera.Indices;
		mayaEsfera.Indices = 0;
	}

	~Skydome()
	{}

	void Draw(Camera* camera, mat4 projection)
	{
		_Shader->use();

		mat4 view = glm::mat4(glm::mat3(camera->GetView()));
		_Shader->setUniform("transform", GetTransformMatrix());
		_Shader->setUniform("view", view);
		_Shader->setUniform("projection", projection);
		_Shader->setUniform("skydome", 0);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glDrawElements(GL_TRIANGLES, CantidadIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		_Shader->stopUsing();
	}

private:

	bool CargarTextura(string ruta, string& logMessage)
	{
		int width, height, nrChannels;
		char* imageData = 0;

		Picture* texture = new Picture(ruta.c_str());
		
		width = texture->GetWidth();
		height = texture->GetHeight();
		imageData = texture->GetBytesArray();

		glGenTextures(1, &TextureID);

		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texture;

		return true;
	}
};

#endif // !_SKYDOME_
