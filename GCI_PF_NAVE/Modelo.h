#pragma once

#ifndef _MODELO_
#define _MODELO_

#include <string>
#include <exception>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include "Geometria.h"
#include "Picture.h"
#include "funciones.h"
#include "Camera.h"
#include "VectorRR.h"

using namespace std;
using namespace glm;

class Modelo
	: public Geometria, Maya
{

private:

	unsigned int TextureID = 0;
	unsigned int NormalMapID = 0;
	unsigned int SpecularMapID = 0;

public:

	void Draw(Camera* camara, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		_Shader->use();

		SetShaderValues(camara, projection, skyColor, lightPosition);
		SetTexturesValues(_Shader);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, CantidadIndices, GL_UNSIGNED_INT, 0);

		_Shader->stopUsing();

		return;
	}

private:

	virtual void SetShaderValues(Camera* camara, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		_Shader->setUniform("worldMatrix", GetTransformMatrix());
		_Shader->setUniform("viewMatrix", camara->GetView());
		_Shader->setUniform("projectionMatrix", projection);

		_Shader->setUniform("skyColor", skyColor);
		_Shader->setUniform("lightPos", lightPosition);
		_Shader->setUniform("viewPos", camara->GetPosition());
	}

	virtual void SetTexturesValues(Shader* shader)
	{
		if (!shader->isInUse())
			return;

		shader->setUniform("shaderTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TextureID);

		// si no tiene mapa de normal no lo cargamos
		if (NormalMapID != 0)
		{
			shader->setUniform("shaderNormal", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, NormalMapID);
		}

		// si no tiene textura de specular no lo cargamos
		if (SpecularMapID != 0)
		{
			shader->setUniform("shaderSpecular", 2);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, SpecularMapID);
		}
	}

	bool CargarTextura(unsigned int& bufferId, string ruta, string& logMessage)
	{
		Picture* textura = new Picture(ruta.c_str());

		glGenTextures(1, &bufferId);
		glBindTexture(GL_TEXTURE_2D, bufferId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textura->GetWidth(), textura->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textura->GetBytesArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		delete textura;

		return true;
	}

	bool CargarModeloAnt2(string ruta, string& logMessage)
	{
		/*
		struct vector3 {
			float x, y, z;
		};

		struct vector2 {
			float u, v;
		};

		vector<vector3> vertices;
		vector<vector2> uvs;
		vector<vector3> normales;
		vector<int> vertexIndices;
		vector<int> uvIndices;
		vector<int> normalIndices;

		errno_t err;
		FILE* file;
		err = fopen_s(&file, ruta.c_str(), "r");
		if (file == NULL) {
			logMessage = "Impossible to open the file !";
			return false;
		}

		while (1)
		{
			char lineHeader[128];
			// Lee la primera palabra de la línea
			int res = fscanf_s(file, "%s", lineHeader, 128);
			if (res == EOF)
				break; // EOF = End Of File, es decir, el final del archivo. Se finaliza el ciclo.

			if (strcmp(lineHeader, "v") == 0) {
				vector3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				vector2 uv;
				fscanf_s(file, "%f %f\n", &uv.u, &uv.v);
				uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				vector3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normales.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					logMessage = "File can't be read by our simple parser : ( Try exporting with other options";
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		//_Maya = new Vertices[vertices.size()];
		//_Indices = new unsigned int[vertexIndices.size()];

		vector<Vertices> tempoVertices;
		vector<unsigned int> tempoIndices;

		for (int i = 0; i < vertexIndices.size(); i++)
		{

			Vertices aux;
			aux.Posx = vertices[vertexIndices[i] - 1].x;
			aux.Posy = vertices[vertexIndices[i] - 1].y;
			aux.Posz = vertices[vertexIndices[i] - 1].z;
			aux.Normx = normales[normalIndices[i] - 1].x;
			aux.Normy = normales[normalIndices[i] - 1].y;
			aux.Normz = normales[normalIndices[i] - 1].z;
			aux.u = uvs[uvIndices[i] - 1].u;
			aux.v = 1 - uvs[uvIndices[i] - 1].v; //por que esta volteada la textura? no lo se, por eso le reste de 1
			//para enderezarla

			tempoVertices.push_back(aux);
		}

		_Maya = new Vertices[tempoVertices.size()];
		Indices = new unsigned int[tempoVertices.size()];

		for (int i = 0; i < tempoVertices.size(); i++)
		{
			_Maya[i] = tempoVertices[i];

			Indices[i] = i;
		}

		CantidadVertices = tempoVertices.size();
		CantidadIndices = tempoVertices.size();

		for (int i = 0; i < CantidadIndices; i += 3)
		{
			vector3 edge1;
			edge1.x = _Maya[vertexIndices[i] - 1 + 2].Posx - _Maya[vertexIndices[i] - 1].Posx;
			edge1.y = _Maya[vertexIndices[i] - 1 + 2].Posy - _Maya[vertexIndices[i] - 1].Posy;
			edge1.z = _Maya[vertexIndices[i] - 1 + 2].Posz - _Maya[vertexIndices[i] - 1].Posz;
			vector3 edge2;
			edge2.x = _Maya[vertexIndices[i] - 1 + 1].Posx - _Maya[vertexIndices[i] - 1].Posx;
			edge2.y = _Maya[vertexIndices[i] - 1 + 1].Posy - _Maya[vertexIndices[i] - 1].Posy;
			edge2.z = _Maya[vertexIndices[i] - 1 + 1].Posz - _Maya[vertexIndices[i] - 1].Posz;

			float d1 = sqrt(_Maya[vertexIndices[i] - 1 + 2].u * _Maya[vertexIndices[i] - 1 + 2].u
				+ _Maya[vertexIndices[i] - 1 + 2].v * _Maya[vertexIndices[i] - 1 + 2].v);

			float d2 = sqrt(_Maya[vertexIndices[i] - 1].u * _Maya[vertexIndices[i] - 1].u
				+ _Maya[vertexIndices[i] - 1].v * _Maya[vertexIndices[i] - 1].v);

			float d3 = sqrt(_Maya[vertexIndices[i] - 1 + 1].u * _Maya[vertexIndices[i] - 1 + 1].u
				+ _Maya[vertexIndices[i] - 1 + 1].v * _Maya[vertexIndices[i] - 1 + 1].v);

			vector2 deltaUV1;

			float ddu1, ddu2;
			float ddv1, ddv2;
			float ddu3, ddv3;

			if (d1 > 0.0001)
				ddu1 = _Maya[vertexIndices[i] - 1 + 2].u / d1;
			else
				ddu1 = 1000;

			if (d2 > 0.0001)
				ddu2 = _Maya[vertexIndices[i] - 1].u / d2;
			else
				ddu2 = 1000;

			if (d1 > 0.0001)
				ddv1 = _Maya[vertexIndices[i] - 1 + 2].v / d1;
			else
				ddv1 = 1000;

			if (d2 > 0.0001)
				ddv2 = _Maya[vertexIndices[i] - 1].v / d2;
			else
				ddv2 = 1000;

			if (d3 > 0.0001)
				ddu3 = _Maya[vertexIndices[i] - 1 + 1].u / d3;
			else
				ddu3 = 1000;

			if (d3 > 0.0001)
				ddv3 = _Maya[vertexIndices[i] - 1 + 1].v / d3;
			else
				ddv3 = 1000;

			deltaUV1.u = ddu1 - ddu2;
			deltaUV1.v = ddv1 - ddv2;

			vector2 deltaUV2;
			deltaUV2.u = ddu3 - ddu2;
			deltaUV2.v = ddv3 - ddv2;

			float denom = (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);

			if (abs(denom) < 0.0001)
				denom = 0.001;

			float f = 1.0f / denom;

			vector3 tangent;

			tangent.x = f * (deltaUV2.v * edge1.x - deltaUV1.v * edge2.x);
			tangent.y = f * (deltaUV2.v * edge1.y - deltaUV1.v * edge2.y);
			tangent.z = f * (deltaUV2.v * edge1.z - deltaUV1.v * edge2.z);

			Normaliza((float*)&tangent);

			_Maya[Indices[i]].Tx = tangent.x;
			_Maya[Indices[i]].Ty = tangent.y;
			_Maya[Indices[i]].Tz = tangent.z;

			_Maya[Indices[i] + 1].Tx = tangent.x;
			_Maya[Indices[i] + 1].Ty = tangent.y;
			_Maya[Indices[i] + 1].Tz = tangent.z;

			_Maya[Indices[i] + 2].Tx = tangent.x;
			_Maya[Indices[i] + 2].Ty = tangent.y;
			_Maya[Indices[i] + 2].Tz = tangent.z;
		}
		*/

		return true;
	}

	bool CargarModelo(string ruta, string& logMessage)
	{
		struct vector3 
		{
			float x, y, z;
		};

		struct vector2 
		{
			float u, v;
		};

		vector<vector3> vertices;
		vector<vector2> uvs;
		vector<vector3> normales;
		vector<int> vertexIndices;
		vector<int> uvIndices;
		vector<int> normalIndices;

		FILE* file;
		fopen_s(&file, ruta.c_str(), "r");

		if (file == NULL) 
		{
			logMessage = "No se pudo abrir el archivo del modelo: " + ruta;
			return false;
		}

		while (!false)
		{
			char lineHeader[128];
			int res = fscanf_s(file, "%s", lineHeader, 128);

			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0) 
			{
				vector3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}

			if (strcmp(lineHeader, "vt") == 0) 
			{
				vector2 uv;
				fscanf_s(file, "%f %f\n", &uv.u, &uv.v);
				uvs.push_back(uv);
			}

			if (strcmp(lineHeader, "vn") == 0) 
			{
				vector3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normales.push_back(normal);
			}

			if (strcmp(lineHeader, "f") == 0) 
			{
				string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
					&vertexIndex[0], &uvIndex[0], &normalIndex[0],
					&vertexIndex[1], &uvIndex[1], &normalIndex[1],
					&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9) 
				{
					logMessage = "El archivo del modelo tiene un formato no compatible";
					return false;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		CantidadVertices = vertices.size();
		CantidadIndices = vertexIndices.size();

		_Maya = new Vertices[CantidadVertices];
		Indices = new unsigned int[CantidadIndices];

		for (int i = 0; i < CantidadIndices; i++)
		{
			_Maya[vertexIndices[i] - 1].Posx = vertices[vertexIndices[i] - 1].x;
			_Maya[vertexIndices[i] - 1].Posy = vertices[vertexIndices[i] - 1].y;
			_Maya[vertexIndices[i] - 1].Posz = vertices[vertexIndices[i] - 1].z;

			_Maya[vertexIndices[i] - 1].Normx = normales[normalIndices[i] - 1].x;
			_Maya[vertexIndices[i] - 1].Normy = normales[normalIndices[i] - 1].y;
			_Maya[vertexIndices[i] - 1].Normz = normales[normalIndices[i] - 1].z;

			_Maya[vertexIndices[i] - 1].u = uvs[uvIndices[i] - 1].u;
			_Maya[vertexIndices[i] - 1].v = uvs[uvIndices[i] - 1].v * -1.0f;
		}

		for (int i = 0; i < CantidadIndices; i++)
		{
			Indices[i] = vertexIndices[i] - 1;
		}

		for (int i = 0; i < CantidadIndices; i += 3)
		{
			Vertices* vert01 = &_Maya[vertexIndices[i] - 1];
			Vertices* vert02 = &_Maya[vertexIndices[i]];
			Vertices* vert03 = &_Maya[vertexIndices[i] + 1];

			if (CantidadVertices <= vertexIndices[i] - 1)
			{
				vert01 = new Vertices;
			}

			if (CantidadVertices <= vertexIndices[i])
			{
				vert02 = new Vertices;
			}

			if (CantidadVertices <= vertexIndices[i] + 1)
			{
				vert03 = new Vertices;
			}

			CalcTanBin(*vert01, *vert02, *vert03);
		}

		return true;
	}

	void CalcTanBin(Vertices& vertex0, Vertices& vertex1, Vertices& vertex2) 
	{
		vec3 posVertex0 = vec3(vertex0.Posx, vertex0.Posy, vertex0.Posz);
		vec3 posVertex1 = vec3(vertex1.Posx, vertex1.Posy, vertex1.Posz);
		vec3 posVertex2 = vec3(vertex2.Posx, vertex2.Posy, vertex2.Posz);

		vec3 normal = cross(posVertex1 - posVertex0, posVertex2 - posVertex0);

		vec3 deltaPos;

		if (vertex0.Posx == vertex1.Posx
			&& vertex0.Posy == vertex1.Posy
			&& vertex0.Posz == vertex1.Posz)
		{
			deltaPos = posVertex2 - posVertex0;
		}
		else
		{
			deltaPos = posVertex1 - posVertex0;
		}

		vec3 tangent, binormal;

		tangent = deltaPos / 1.0f;
		tangent = tangent - (dot(normal, tangent) * normal);
		tangent = normalize(tangent);

		binormal = cross(tangent, normal);
		binormal = normalize(binormal);

		// tangents
		vertex0.Tx = tangent.x;
		vertex0.Ty = tangent.y;
		vertex0.Tz = tangent.z;

		vertex1.Tx = tangent.x;
		vertex1.Ty = tangent.y;
		vertex1.Tz = tangent.z;

		vertex2.Tx = tangent.x;
		vertex2.Ty = tangent.y;
		vertex2.Tz = tangent.z;

		// binormales
		vertex0.Bx = binormal.x;
		vertex0.By = binormal.y;
		vertex0.Bz = binormal.z;

		vertex1.Bx = binormal.x;
		vertex1.By = binormal.y;
		vertex1.Bz = binormal.z;

		vertex2.Bx = binormal.x;
		vertex2.By = binormal.y;
		vertex2.Bz = binormal.z;
	}

public:

	Modelo(string rutaModelo, string rutaTextura, string rutaMapaNormal, string rutaMapaSpecular,
		string rutaVertShader, string rutaFragShader)
		: Geometria(rutaVertShader.c_str(), rutaFragShader.c_str())
	{
		string logMessage = "";

		if (!CargarModelo(rutaModelo, logMessage))
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage);
			throw exception(logMessage.c_str());
			PostQuitMessage(0);
		}

		if (!CargarTextura(TextureID, rutaTextura, logMessage))
		{
			Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage);
			PostQuitMessage(0);
		}

		if (rutaMapaNormal != "")
		{
			if (!CargarTextura(NormalMapID, rutaMapaNormal, logMessage))
			{
				Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage);
				PostQuitMessage(0);
			}
		}

		if (rutaMapaSpecular != "")
		{
			if (!CargarTextura(SpecularMapID, rutaMapaSpecular, logMessage))
			{
				Fun::ShowMessage(MSG_CAPTION_ERROR, logMessage);
				PostQuitMessage(0);
			}
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, CantidadVertices * sizeof(Vertices), _Maya, GL_STATIC_DRAW);

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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, CantidadIndices * sizeof(unsigned int), Indices, GL_STATIC_DRAW);
	}

	~Modelo()
	{
		glDeleteTextures(1, &TextureID);
		glDeleteTextures(1, &NormalMapID);
		glDeleteTextures(1, &SpecularMapID);
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

#endif // !_MODELO_