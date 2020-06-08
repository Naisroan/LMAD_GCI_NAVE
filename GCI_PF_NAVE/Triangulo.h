#pragma once

#ifndef _TRIANGULO_
#define _TRIANGULO_

#include "Geometria.h"
#include "Camera.h"

class Triangulo 
	: public Geometria
{

public:

	Triangulo()
		: Geometria("res/shaders/Triangulo.vert", "res/shaders/Triangulo.frag")
	{
		// vertices de un triangulo
		// es x, y, z
		// los valores oscilan entre 0 y 1 ya que asi funciona opengl
		static float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, // parte inferior izquierda
			0.5f, -0.5f, 0.0f, // parte inferior derecha
			0.0f, 0.5f, 0.0f // parte media superior
		};

		// NOTA 1: los shaders se crean en la clase Primitivo2D

		// NOTA 2: hacemos uso del VBO ya que cuesta mandar info a la GPU, 
		// asi que, reunimos toda la informacion (todos los vertices)
		// y lo enviamos a traves de la variable VBO que funcionará como un arreglo ya en la gpu

		// NOTA 3: además usamos el VAO que vendría siendo el arreglo de VBOs

		// >> COMENZAMOS LA CONFIGURACION DEL TRIANGULO

		// 0.- generamos el VAO (Vertex Array Object)
		//		1er param: 
		glGenVertexArrays(1, &VAO);

		// 1.- lo enlazamos
		glBindVertexArray(VAO);

		// 2.- generamos el VBO (Vertex Buffer Object)
		//		1er param: 
		glGenBuffers(1, &VBO);

		// 3.- enlazamos e indicamos que el tipo de buffer sera un array (array de vertices jeje)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// 4.- le mandamos la data
		//		1er param: tipo de buffer en donde pasaremos los datos
		//		2do param: el tamaño de la data
		//		3er param: la data
		//		4to patam: le indicamos que la data conserva su posicion 
		//				también estan GL_STREAM_DRAW y GL_DYNAMIC_DRAW
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 5.- mencionamos que el vbo funciona como un arreglo, es hora de decirle como se comportará
		//		1er param: le decimos la ubicacion del atributo 'aPos' que es layout (location = 0), es decir, 0
		//		2do param: es el tamaño o como se compone (x, y, z), es decir, 3 o un vec3, que es el tipo de dato de aPos
		//		3er param: el tipo de datos del atributo
		//		4to param: si queremos normalizar los datos (entre 0 y 1)
		//		5to param: el espacio entre los atributos, o mas bien, el tamaño
		//				como es de 3 en 3 el arreglo de vertices, y es flotante...,
		//				si serian 4 datos (r, g, b, a) seria 4 * el tipo de dato
		//		6to param: en donde comienza la posición de los datos, si metemos mas atributos esto cambia
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// 6.- habilitamos el atributo que acabamos de configurar 'aPos'
		glEnableVertexAttribArray(0);

		// 7.- desusamos
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Draw(mat4 view, mat4 projection)
	{
		// 0.- usamos el shader
		_Shader->use();
		_Shader->setUniform("transform", GetTransformMatrix());
		_Shader->setUniform("view", view);
		_Shader->setUniform("projection", projection);

		// 1.- le indicamos el VAO que utilizaremos, es el que contiene los VBO, este caso 1
		glBindVertexArray(VAO);

		// 2.- dibujamos :D
		//		1er param: la forma en que se van a dibujar
		//		2do param: el indice inicial
		//      3er param: numero de vertices que se quieren unir
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 3.- desusamos el VAO
		glBindVertexArray(0);

		// 4.- desusamos el shader
		_Shader->stopUsing();
	}

};

#endif // !_TRIANGULO_
