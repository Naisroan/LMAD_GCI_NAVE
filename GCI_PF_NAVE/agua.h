#pragma once

#ifndef __AGUA__
#define __AGUA__

#include <string>
#include "Geometria.h"
#include "Picture.h"
#include "VectorRR.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Agua
	: public Geometria, Maya
{

private:

	static float MovementVertex;
	static float MovementTexture;

	const float DEFAULT_ANCHO = 500.0f;
	const float DEFAULT_PROFUNIDAD = 500.0f;

	float DeltaX;
	float DeltaZ;
	int VertexX;
	int VertexZ;

	unsigned int TextureID;
	unsigned int TextureSpecularID;

	Maya _MayaAgua;

public:

	Agua(string rutaTextura, string rutaAltura, string rutaTexturaSpecular, string rutaVertShader, string rutaFragShader)
		: Geometria(rutaVertShader.c_str(), rutaFragShader.c_str())
	{
		Picture* texAltura = new Picture(rutaAltura.c_str());

		_MayaAgua = Plano(texAltura->GetWidth(), texAltura->GetHeight(),
			DEFAULT_ANCHO, DEFAULT_PROFUNIDAD, (unsigned char*)texAltura->GetBytesArray(), 100.0f);

		DeltaX = DEFAULT_ANCHO / texAltura->GetWidth();
		DeltaZ = DEFAULT_PROFUNIDAD / texAltura->GetHeight();
		VertexX = texAltura->GetWidth();
		VertexZ = texAltura->GetHeight();

		delete texAltura;

		GenerarTexturas(rutaTextura, rutaTexturaSpecular);
		GenerarVAO();
	}

	void Draw(Camera* camara, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_Shader->use();

		_Shader->setUniform("worldMatrix", GetTransformMatrix());
		_Shader->setUniform("viewMatrix", camara->GetView());
		_Shader->setUniform("projectionMatrix", projection);

		_Shader->setUniform("skyColor", skyColor);
		_Shader->setUniform("viewPos", camara->GetPosition());
		_Shader->setUniform("lightPos", lightPosition);

		_Shader->setUniform("shaderTexture", 0);
		_Shader->setUniform("shaderSpecular", 1);

		_Shader->setUniform("movementVertex", Agua::MovementVertex);
		_Shader->setUniform("movementTexture", Agua::MovementTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureSpecularID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, CantidadIndices, GL_UNSIGNED_INT, 0);

		static bool toUp = true;
		
		if (Agua::MovementVertex >= 1.5f)
			toUp = false;

		if (Agua::MovementVertex <= 0.5f)
			toUp = true;

		Agua::MovementTexture += 0.02;
		Agua::MovementVertex += toUp ? 0.01f : -0.01f;

		_Shader->stopUsing();

		glDisable(GL_BLEND);
	}

	float GetHeightFromPoint(vec3 position)
	{
		//obtenemos el indice pero podria incluir una fraccion
		float indicefx = (position.x + DEFAULT_ANCHO / 2) / DeltaX;
		float indicefz = (position.z + DEFAULT_PROFUNIDAD / 2) / DeltaZ;

		//nos quedamos con solo la parte entera del indice
		int indiceix = (int)indicefx;
		int indiceiz = (int)indicefz;

		//nos quedamos con solo la fraccion del indice
		float difx = indicefx - indiceix;
		float difz = indicefz - indiceiz;

		float altura;
		float D;

		//el cuadro del terreno esta formado por dos triangulos, si difx es mayor que dify 
		//entonces estamos en el triangulo de abajo en caso contrario arriba
		if (difx > difz)
		{
			//obtenemos el vector 1 de dos que se necesitan
			VectorRR v1(_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posx - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posy - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posz - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			//obtenemos el vector 2 de dos
			VectorRR v2(_MayaAgua._Maya[indiceix + 1 + indiceiz * VertexX].Posx - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaAgua._Maya[indiceix + 1 + indiceiz * VertexX].Posy - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaAgua._Maya[indiceix + 1 + indiceiz * VertexX].Posz - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
			//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
			VectorRR normalPlano = Cruz(v1, v2);

			//entonces solo falta calcular D
			D = -1 * (normalPlano.X * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx +
				normalPlano.Y * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy +
				normalPlano.Z * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			//sustituyendo obtenemos la altura de contacto en el terreno
			altura = ((-normalPlano.X * position.x - normalPlano.Z * position.z - D) / normalPlano.Y);
		}
		else
		{
			VectorRR v1(_MayaAgua._Maya[indiceix + (indiceiz + 1) * VertexX].Posx - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaAgua._Maya[indiceix + (indiceiz + 1) * VertexX].Posy - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaAgua._Maya[indiceix + (indiceiz + 1) * VertexX].Posz - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			VectorRR v2(_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posx - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posy - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaAgua._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posz - _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			VectorRR normalPlano = Cruz(v1, v2);

			D = -1 * (normalPlano.X * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posx +
				normalPlano.Y * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posy +
				normalPlano.Z * _MayaAgua._Maya[indiceix + indiceiz * VertexX].Posz);

			altura = ((-normalPlano.X * position.x - normalPlano.Z * position.z - D) / normalPlano.Y);
		}

		return altura;
	}

private:

	void GenerarVAO()
	{
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, CantidadVertices * sizeof(Vertices), _MayaAgua._Maya, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);  // Vertex position.
		glEnableVertexAttribArray(1);  // Normals.
		glEnableVertexAttribArray(2);  // Texture coordinates.
		glEnableVertexAttribArray(3);  // Tangent.
		glEnableVertexAttribArray(4);  // BiNormals.

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, CantidadIndices * sizeof(unsigned int),
			_MayaAgua.Indices, GL_STATIC_DRAW);

		delete[] _MayaAgua.Indices;
		_MayaAgua.Indices = 0;
	}

	void GenerarTexturas(string rutaTextura, string rutaTexturaSpecular)
	{
		Picture* textura = new Picture(rutaTextura.c_str());

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textura->GetWidth(), textura->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, textura->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete textura;

		Picture* texturaSpecular = new Picture(rutaTexturaSpecular.c_str());

		glGenTextures(1, &TextureSpecularID);
		glBindTexture(GL_TEXTURE_2D, TextureSpecularID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturaSpecular->GetWidth(), texturaSpecular->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texturaSpecular->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texturaSpecular;
	}

public:

	~Agua()
	{
		delete[] _MayaAgua._Maya;
		_MayaAgua._Maya = 0;

		glDeleteTextures(1, &TextureID);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &EBO);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAO);
	}

};

float Agua::MovementTexture = 0.0f;
float Agua::MovementVertex = 0.5f;

#endif // !__AGUA__