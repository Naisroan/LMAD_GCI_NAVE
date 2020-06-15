#pragma once

#ifndef _GEOMETRIA_
#define _GEOMETRIA_

#include <Windows.h>
#include <vector>
#include <corecrt_math_defines.h>
#include <string>
#include "includes_opengl.h"
#include "VectorRR.h"
#include "Shader.h"
#include "Maya.h"

using namespace std;
using namespace glm;

class Geometria : protected VectorRR
{

private:

	struct Transform
	{
		mat4 Matrix;
		vec3 Position;
		vec3 Rotation;
		vec3 Scale;
	};

	Transform _Transform;

protected:

	const char* RUTA_ARCHIVO_VERTEXSHADER = "";
	const char* RUTA_ARCHIVO_FRAGMENTSHADER = "";

	unsigned int VAO = -1;
	unsigned int VBO = -1;
	unsigned int EBO = -1;

	int CantidadVertices;
	int CantidadIndices;

	vector<ShaderFile> ShaderFiles;
	Shader* _Shader = 0;

	Maya BillBoard(float escala)
	{
		CantidadVertices = 4;
		CantidadIndices = 6;

		// Cargamos la estructura con los espacios de memoria necesarios
		Vertices* verticesxyzSD = new Vertices[CantidadVertices];
		unsigned int* indices = new unsigned int[CantidadIndices];

		// generamos un objeto para poder transportar los punteros
		Maya _maya;

		// a darle que es mole de olla!
		verticesxyzSD[0].Posx = -0.5 * escala;
		verticesxyzSD[0].Posy = 0 * escala;
		verticesxyzSD[0].Posz = 0 * escala;

		verticesxyzSD[0].Normx = 0;
		verticesxyzSD[0].Normy = 0;
		verticesxyzSD[0].Normz = 1;

		verticesxyzSD[0].u = 1;
		verticesxyzSD[0].v = 1;

		verticesxyzSD[1].Posx = -0.5 * escala;
		verticesxyzSD[1].Posy = 1 * escala;
		verticesxyzSD[1].Posz = 0 * escala;

		verticesxyzSD[1].Normx = 0;
		verticesxyzSD[1].Normy = 0;
		verticesxyzSD[1].Normz = 1;

		verticesxyzSD[1].u = 1;
		verticesxyzSD[1].v = 0;

		verticesxyzSD[2].Posx = 0.5 * escala;
		verticesxyzSD[2].Posy = 1 * escala;
		verticesxyzSD[2].Posz = 0 * escala;

		verticesxyzSD[2].Normx = 0;
		verticesxyzSD[2].Normy = 0;
		verticesxyzSD[2].Normz = 1;

		verticesxyzSD[2].u = 0;
		verticesxyzSD[2].v = 0;

		verticesxyzSD[3].Posx = 0.5 * escala;
		verticesxyzSD[3].Posy = 0 * escala;
		verticesxyzSD[3].Posz = 0 * escala;

		verticesxyzSD[3].Normx = 0;
		verticesxyzSD[3].Normy = 0;
		verticesxyzSD[3].Normz = 1;

		verticesxyzSD[3].u = 0;
		verticesxyzSD[3].v = 1;

		indices[0] = 0;
		indices[1] = 2;
		indices[2] = 1;
		indices[3] = 0;
		indices[4] = 3;
		indices[5] = 2;

		// una vez generados los damos a conocer a traves del objeto "salida"
		_maya._Maya = verticesxyzSD;
		_maya.Indices = indices;

		return _maya;
	}

	Maya Esfera(int stacks, int slices, float radio, float inicio, float final)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices* verticesxyzSD = new Vertices[stacks * slices];
		CantidadVertices = stacks * slices;
		unsigned int* indices = new unsigned int[(stacks - 1) * (slices - 1) * 6];
		CantidadIndices = (stacks - 1) * (slices - 1) * 6;
		//generamos un objeto para poder transportar los punteros
		Maya salida;
		//a darle que es mole de olla!
		for (int i = 0; i < slices; i++)
		{
			for (int j = 0; j < stacks; j++)
			{
				int indice = (i * stacks + j);
				verticesxyzSD[indice].Posx = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					cos(2.0 * M_PI * (double)i / (slices - 1));
				verticesxyzSD[indice].Posy = radio * sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Posz = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					sin(2.0 * M_PI * (double)i / (slices - 1));

				verticesxyzSD[indice].Normx = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					cos(2.0 * M_PI * (double)i / (slices - 1));
				verticesxyzSD[indice].Normy = sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
				verticesxyzSD[indice].Normz = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
					sin(2.0 * M_PI * (double)i / (slices - 1));

				verticesxyzSD[indice].u = (float)1 * (1.0 - (float)i / (float)(stacks - 1));
				verticesxyzSD[indice].v = (float)1 * (1.0 - (float)j / (float)(slices - 1));
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < slices - 1; i++)
		{
			for (int j = 0; j < stacks - 1; j++)
			{
				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
				indices[indice++] = i * stacks + j + 1;

				indices[indice++] = i * stacks + j;
				indices[indice++] = (i + 1) * stacks + j;
				indices[indice++] = (i + 1) * stacks + j + 1;
			}
		}

		//una vez generados los damos a conocer a traves del objeto "salida"
		salida._Maya = verticesxyzSD;
		salida.Indices = indices;

		return salida;
	}

	Maya Plano(int vertx, int vertz, float anchof, float profz)
	{
		//Cargamos la estructura con los espacios de memoria necesarios
		Vertices* verticesxyzSD = new Vertices[vertx * vertz];
		unsigned int* indices = new unsigned int[(vertx - 1) * (vertz - 1) * 6];
		CantidadVertices = vertx * vertz;
		CantidadIndices = (vertx - 1) * (vertz - 1) * 6;
		//es la separacion entre vertices, se le resta 1 para que el lado correcto
		//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
		//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
		//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
		float deltax = anchof / (vertx - 1);
		float deltaz = profz / (vertz - 1);

		//crea los vertices
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z * vertx + x].Posx = (float)x * deltax;
				verticesxyzSD[z * vertx + x].Posy = 0.0;
				verticesxyzSD[z * vertx + x].Posz = (float)z * deltaz;

				//carga las normales con cero
				verticesxyzSD[z * vertx + x].Normx = 0.0;
				verticesxyzSD[z * vertx + x].Normy = 1.0;
				verticesxyzSD[z * vertx + x].Normz = 0.0;
			}
		}

		//calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z * vertx + x].u = (float)x / (vertx - 1);
				verticesxyzSD[z * vertx + x].v = (float)z / (vertz - 1);
			}
		}

		VectorRR aux;
		//crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx,
					&verticesxyzSD[z * vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + x].Posx,
					&verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
			}
		}

		//Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z * vertx + x].Normx);
			}
		}

		//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		//a un algoritmo con un un grupo de indices
		int indice = 0;
		for (int i = 0; i < vertz - 1; i++)
		{
			for (int j = 0; j < vertx - 1; j++)
			{
				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
				indices[indice++] = i * vertz + j + 1;

				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
			}
		}

		//generamos un objeto para poder transportar los punteros

		Maya salida;

		salida._Maya = verticesxyzSD;
		salida.Indices = indices;

		return salida;
	}

	Maya Plano(int vertx, int vertz, float anchof, float profz, unsigned char* altura, float tile = 1)
	{
		CantidadVertices = vertx * vertz;
		CantidadIndices = (vertx - 1) * (vertz - 1) * 6;
		
		// Cargamos la estructura con los espacios de memoria necesarios
		Vertices* verticesxyzSD = new Vertices[CantidadVertices];
		unsigned int* indices = new unsigned int[CantidadIndices];

		// es la separacion entre vertices, se le resta 1 para que el lado correcto
		// imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
		// de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
		// seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
		float deltax = anchof / (vertx - 1);
		float deltaz = profz / (vertz - 1);

		// crea los vertices
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z * vertx + x].Posx = (float)x * deltax - anchof / 2.0;
				verticesxyzSD[z * vertx + x].Posy = (float)altura[z * vertx * 4 + x * 4] / 6.0;
				verticesxyzSD[z * vertx + x].Posz = (float)z * deltaz - profz / 2.0;
			}
		}

		// calcula los uv's
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				verticesxyzSD[z * vertx + x].u = (float)(x * tile) / (vertx - 1);
				verticesxyzSD[z * vertx + x].v = (float)(z * tile) / (vertz - 1);
			}
		}

		VectorRR aux;

		// crea las normales
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				Copia(aux, GenNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[z * vertx + (x + 1)].Posx,
					&verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx));

				SumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Normx, &aux.X);

				Copia(aux, GenNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + x + 1].Posx,
					&verticesxyzSD[(z + 1) * vertx + x].Posx));

				SumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Normx, &aux.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.X);
			}
		}

		VectorRR tangente;

		// crea las  tangentes

		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				// primer triangulo
				tangente.X = verticesxyzSD[z * vertx + (x + 1)].Posx - verticesxyzSD[z * vertx + x].Posx;
				tangente.Y = verticesxyzSD[z * vertx + (x + 1)].Posy - verticesxyzSD[z * vertx + x].Posy;
				tangente.Z = verticesxyzSD[z * vertx + (x + 1)].Posz - verticesxyzSD[z * vertx + x].Posz;

				VectorRR aux;
				aux.X = verticesxyzSD[z * vertx + x].Normx;
				aux.Y = verticesxyzSD[z * vertx + x].Normy;
				aux.Z = verticesxyzSD[z * vertx + x].Normz;

				float dotazo = Punto(aux, tangente);

				VectorRR aux2;
				aux2.X = dotazo * aux.X;
				aux2.Y = dotazo * aux.Y;
				aux2.Z = dotazo * aux.Z;

				tangente.X = tangente.X - aux2.X;
				tangente.Y = tangente.Y - aux2.Y;
				tangente.Z = tangente.Z - aux2.Z;

				SumaNormal(&verticesxyzSD[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Tx, &tangente.X);

				// segundo triangulo
				tangente.X = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx - verticesxyzSD[z * vertx + x].Posx;
				tangente.Y = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posy - verticesxyzSD[z * vertx + x].Posy;
				tangente.Z = verticesxyzSD[(z + 1) * vertx + (x + 1)].Posz - verticesxyzSD[z * vertx + x].Posz;

				SumaNormal(&verticesxyzSD[z * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Tx, &tangente.X);
				SumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Tx, &tangente.X);
			}
		}

		// Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z * vertx + x].Normx);
				Normaliza(&verticesxyzSD[z * vertx + x].Tx);
			}
		}

		VectorRR bitangente, tan, norm;

		// crea las  tangentes
		for (int z = 0; z < (vertz - 1); z++)
		{
			for (int x = 0; x < (vertx - 1); x++)
			{
				bitangente.X = -(verticesxyzSD[z * vertx + x].Ty * verticesxyzSD[z * vertx + x].Normz - verticesxyzSD[z * vertx + x].Tz *
					verticesxyzSD[z * vertx + x].Normy);
				bitangente.Y = -(verticesxyzSD[z * vertx + x].Tz * verticesxyzSD[z * vertx + x].Normx - verticesxyzSD[z * vertx + x].Tx *
					verticesxyzSD[z * vertx + x].Normz);
				bitangente.Z = -(verticesxyzSD[z * vertx + x].Tx * verticesxyzSD[z * vertx + x].Normy - verticesxyzSD[z * vertx + x].Ty *
					verticesxyzSD[z * vertx + x].Normx);

				SumaNormal(&verticesxyzSD[z * vertx + x].Bx, &bitangente.X);
			}
		}

		// Normaliza las normales
		for (int z = 0; z < vertz; z++)
		{
			for (int x = 0; x < vertx; x++)
			{
				Normaliza(&verticesxyzSD[z * vertx + x].Bx);
			}
		}

		// ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
		// a un algoritmo con un un grupo de indices
		int indice = 0;

		for (int i = 0; i < vertz - 1; i++)
		{
			for (int j = 0; j < vertx - 1; j++)
			{
				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
				indices[indice++] = i * vertz + j + 1;

				indices[indice++] = i * vertz + j;
				indices[indice++] = (i + 1) * vertz + j;
				indices[indice++] = (i + 1) * vertz + j + 1;
			}
		}

		// generamos un objeto salida para poder transportar los punteros
		Maya salida;

		salida._Maya = verticesxyzSD;
		salida.Indices = indices;

		return salida;
	}

public:

	Geometria(const char* rutaArchivoVertexShader, const char* rutaArchivoFragmentShader)
	{
		RUTA_ARCHIVO_VERTEXSHADER = rutaArchivoVertexShader;
		RUTA_ARCHIVO_FRAGMENTSHADER = rutaArchivoFragmentShader;

		_Shader = new Shader(rutaArchivoVertexShader, rutaArchivoFragmentShader);

		CantidadVertices = 0;
		CantidadIndices = 0;

		_Transform.Matrix = mat4(1.0f);
		_Transform.Position = vec3(0.0f, 0.0f, 0.0f);
		_Transform.Rotation = vec3(0.0f, 0.0f, 0.0f);
		_Transform.Scale = vec3(1.0f, 1.0f, 1.0f);

		SetTransformations();
	}

	Geometria()
	{
		_Transform.Matrix = mat4(1.0f);
		_Transform.Position = vec3(0.0f, 0.0f, 0.0f);
		_Transform.Rotation = vec3(0.0f, 0.0f, 0.0f);
		_Transform.Scale = vec3(1.0f, 1.0f, 1.0f);

		SetTransformations();
	}

	~Geometria()
	{
		if (VAO != -1)
			glDeleteVertexArrays(1, &VAO);

		if (VBO != -1)
			glDeleteBuffers(1, &VBO);

		if (EBO != -1)
			glDeleteBuffers(1, &EBO);

		if (_Shader)
		{
			delete _Shader;
		}
	}

	mat4 GetTransformMatrix()
	{
		return _Transform.Matrix;
	}

	vec3 GetPosition()
	{
		return _Transform.Position;
	}

	vec3 GetRotation()
	{
		return _Transform.Rotation;
	}

	vec3 GetScale()
	{
		return _Transform.Scale;
	}

	void SetPosition(vec3 position)
	{
		_Transform.Position = position;

		SetTransformations();
	}

	void SetRotation(vec3 rotation)
	{
		rotation.x = radians(rotation.x);
		rotation.y = radians(rotation.y);
		rotation.z = radians(rotation.z);

		_Transform.Rotation = rotation;

		SetTransformations();
	}

	void SetScale(vec3 _scale)
	{
		_Transform.Scale = _scale;

		SetTransformations();
	}

	void Rotate(float angle, vec3 eje)
	{
		if (eje.y == 1.0f)
		{
			_Transform.Rotation = _Transform.Rotation + vec3(0.0f, radians(angle), 0.0f);
		}

		SetTransformations();
	}

private:

	void SetTransformations()
	{
		mat4 translateMatrix = mat4(1.0f);
		mat4 rotationMatrix = mat4(1.0f);
		mat4 scaleMatrix = mat4(1.0f);

		translateMatrix = translate(translateMatrix, _Transform.Position);
		scaleMatrix = scale(scaleMatrix, _Transform.Scale);

		rotationMatrix = rotate(rotationMatrix, _Transform.Rotation.x, vec3(1, 0, 0));
		rotationMatrix = rotate(rotationMatrix, _Transform.Rotation.y, vec3(0, 1, 0));
		rotationMatrix = rotate(rotationMatrix, _Transform.Rotation.z, vec3(0, 0, 1));

		// primero es escalar, rotar y trasladar, pero es al revés, la profe Adriana lo explico
		_Transform.Matrix = translateMatrix * rotationMatrix * scaleMatrix;
	}
};

#endif 
