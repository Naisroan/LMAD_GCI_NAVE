#pragma once

#ifndef _TERRENO_
#define _TERRENO_

#include <string>
#include "Geometria.h"
#include "Picture.h"
#include "VectorRR.h"
#include "Camera.h"

using namespace std;
using namespace glm;

class Terreno
	: public Geometria, Maya
{

private:

	const float DEFAULT_ANCHO = 250.0f;
	const float DEFAULT_PROFUNIDAD = 250.0f;

	float DeltaX;
	float DeltaZ;
	int VertexX;
	int VertexZ;

	unsigned int TextureMultiID;

	unsigned int TextureID;
	unsigned int TextureNormalID;

	unsigned int Texture2ID;
	unsigned int TextureNormal2ID;

	Maya _MayaTerreno;

public:

	Terreno(string rutaMapaAlturas, string rutaTextura, string rutaMapaNormal,
		string rutaTextura2, string rutaMapaNormal02, string rutaMultiTextura, string rutaVertShader, string rutaFragShader)
		: Geometria(rutaVertShader.c_str(), rutaFragShader.c_str())
	{
		Picture* texAltura = new Picture(rutaMapaAlturas.c_str());

		_MayaTerreno = Plano(texAltura->GetWidth(), texAltura->GetHeight(),
			DEFAULT_ANCHO, DEFAULT_PROFUNIDAD, (unsigned char*)texAltura->GetBytesArray(), 1.0f);

		DeltaX = DEFAULT_ANCHO / texAltura->GetWidth();
		DeltaZ = DEFAULT_PROFUNIDAD / texAltura->GetHeight();
		VertexX = texAltura->GetWidth();
		VertexZ = texAltura->GetHeight();

		delete texAltura;

		GenerarTexturas(rutaTextura, rutaMapaNormal, rutaTextura2, 
			rutaMapaNormal02, rutaMultiTextura);

		GenerarVAO();
	}

	void Draw(Camera* camara, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		_Shader->use();

		_Shader->setUniform("worldMatrix", GetTransformMatrix());
		_Shader->setUniform("viewMatrix", camara->GetView());
		_Shader->setUniform("projectionMatrix", projection);

		_Shader->setUniform("skyColor", skyColor);
		_Shader->setUniform("viewPos", camara->GetPosition());
		_Shader->setUniform("lightPos", lightPosition);

		_Shader->setUniform("shaderMultiTex", 0);
		_Shader->setUniform("shaderTexCesped", 1);
		_Shader->setUniform("shaderNormalCesped", 2);
		_Shader->setUniform("shaderTexRoca", 3);
		_Shader->setUniform("shaderNormalRoca", 4);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureMultiID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TextureNormalID);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Texture2ID);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, TextureNormal2ID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, CantidadIndices, GL_UNSIGNED_INT, 0);

		_Shader->stopUsing();
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
			VectorRR v1(_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posx - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posy - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posz - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

			//obtenemos el vector 2 de dos
			VectorRR v2(_MayaTerreno._Maya[indiceix + 1 + indiceiz * VertexX].Posx - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaTerreno._Maya[indiceix + 1 + indiceiz * VertexX].Posy - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaTerreno._Maya[indiceix + 1 + indiceiz * VertexX].Posz - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

			//con el producto punto obtenemos la normal y podremos obtener la ecuacion del plano
			//la parte x de la normal nos da A, la parte y nos da B y la parte z nos da C
			VectorRR normalPlano = Cruz(v1, v2);

			//entonces solo falta calcular D
			D = -1 * (normalPlano.X * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx +
				normalPlano.Y * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy +
				normalPlano.Z * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

			//sustituyendo obtenemos la altura de contacto en el terreno
			altura = ((-normalPlano.X * position.x - normalPlano.Z * position.z - D) / normalPlano.Y);
		}
		else
		{
			VectorRR v1(_MayaTerreno._Maya[indiceix + (indiceiz + 1) * VertexX].Posx - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaTerreno._Maya[indiceix + (indiceiz + 1) * VertexX].Posy - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaTerreno._Maya[indiceix + (indiceiz + 1) * VertexX].Posz - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

			VectorRR v2(_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posx - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx,
				_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posy - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy,
				_MayaTerreno._Maya[indiceix + 1 + (indiceiz + 1) * VertexX].Posz - _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

			VectorRR normalPlano = Cruz(v1, v2);

			D = -1 * (normalPlano.X * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posx +
				normalPlano.Y * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posy +
				normalPlano.Z * _MayaTerreno._Maya[indiceix + indiceiz * VertexX].Posz);

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
		glBufferData(GL_ARRAY_BUFFER, CantidadVertices * sizeof(Vertices), _MayaTerreno._Maya, GL_STATIC_DRAW);

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
			_MayaTerreno.Indices, GL_STATIC_DRAW);

		delete[] _MayaTerreno.Indices;
		_MayaTerreno.Indices = 0;
	}

	void GenerarTexturas(string rutaTextura01, string rutaTexturaNormal01,
		string rutaTextura02, string rutaTexturaNormal02, string rutaMultiTextura)
	{
		// multitextura
		Picture* texMulti = new Picture(rutaMultiTextura.c_str());

		glGenTextures(1, &TextureMultiID);
		glBindTexture(GL_TEXTURE_2D, TextureMultiID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texMulti->GetWidth(), texMulti->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texMulti->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texMulti;

		// textura 1 (Cesped)
		Picture* texTerreno01 = new Picture(rutaTextura01.c_str());

		glGenTextures(1, &TextureID);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texTerreno01->GetWidth(), texTerreno01->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texTerreno01->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texTerreno01;

		// textura normal 1 (Cesped)
		Picture* texNormal01 = new Picture(rutaTexturaNormal01.c_str());

		glGenTextures(1, &TextureNormalID);
		glBindTexture(GL_TEXTURE_2D, TextureNormalID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texNormal01->GetWidth(), texNormal01->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texNormal01->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texNormal01;

		// textura 2 (Roca)
		Picture* texTerreno02 = new Picture(rutaTextura02.c_str());

		glGenTextures(1, &Texture2ID);

		glBindTexture(GL_TEXTURE_2D, Texture2ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texTerreno02->GetWidth(), texTerreno02->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texTerreno02->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texTerreno02;

		// textura normal 2 (Roca)
		Picture* texNormal02 = new Picture(rutaTexturaNormal02.c_str());

		glGenTextures(1, &TextureNormal2ID);

		glBindTexture(GL_TEXTURE_2D, TextureNormal2ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texNormal02->GetWidth(), texNormal02->GetHeight(), 0,
			GL_RGBA, GL_UNSIGNED_BYTE, texNormal02->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete texNormal02;
	}

public:

	~Terreno()
	{
		delete[] _MayaTerreno._Maya;
		_MayaTerreno._Maya = 0;

		glDeleteTextures(1, &TextureID);
		glDeleteTextures(1, &Texture2ID);

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

#endif // !_TERRENO_