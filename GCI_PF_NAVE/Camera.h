#pragma once

#ifndef _CAMERA_
#define _CAMERA_

#include <glm.hpp>
#include <matrix_transform.hpp>
#include <iostream>

using namespace glm;

class Camera
{

private:

	vec3 Position;		// posicion de la camara
	vec3 Rotation;		// rotacion de la camara (angulo x, y, z)

	vec3 AntesColisionPosition;

	float Pitch = 0.0;			// x
	float Yaw = -90.0;			// y
	// float Euler = 0.0f;		// z

	vec3 Target;		// a donde mira la camara
	vec3 Direction;		// la direcion a donde esta mirando la camara

	vec3 Up;			// es el vector que contendrá el eje en donde va a girar la camara
	vec3 Right;			// es la direccion derecha de donde esta la camara
	vec3 Forward;		// es la direccion de enfrente de donde esta la camara
	mat4 ViewMatrix;	// es la matriz de la camara (todos los calculos)

	float Velocity = 3.0f;		// velocidad de la camara
	float RunVelocity = 6.0f;	// velocidad de la camara al correr
	float FlyVelocity = 2.0f;	// velocidad de la camara al volar
	float Sensibilidad = 4.0f;	// sensibilidad de la camara

	bool IsRun = false;
	bool IsFly = true;

	bool CanMove = true;

public:

	Camera()
	{
		Position = vec3(0.0f, 0.0f, 0.0f);
		Rotation = vec3(0.0f, 0.0f, 0.0f);

		Target = vec3(0.0f, 0.0f, 0.0f);
		Direction = normalize(Position - Target);

		Up = vec3(0.0f, 1.0f, 0.0f);
		Right = normalize(cross(Up, Direction));
		Forward = vec3(0.0f, 0.0f, -1.0f);

		SetDirection();
		SetViewMatrix();
	}

	~Camera()
	{
	}

	void SetPosition(vec3 position)
	{
		Position = position;
	}

	void SetRotation(vec3 rotation)
	{
		Rotation = rotation;
	}

	vec3 GetPosition()
	{
		return Position;
	}

	vec3 GetForward()
	{
		return Forward;
	}

	mat4 GetView()
	{
		return ViewMatrix;
	}

	float GetYaw()
	{
		return Yaw;
	}

	float GetVelocity()
	{
		return Velocity;
	}

	float GetFlyVelocity()
	{
		return FlyVelocity;
	}

	float GetRunVelocity()
	{
		return RunVelocity;
	}

	bool GetIsRun()
	{
		return IsRun;
	}

	float GetVelocityActual()
	{
		return IsRun ? RunVelocity : IsFly ? FlyVelocity : Velocity;
	}

	void Move(float vertical, float horizontal, float deltaTime)
	{
		if (!CanMove)
			return;

		float vel = 0.0f;
		
		vel = !IsRun ? Velocity : RunVelocity;
		vel = IsFly ? IsRun ? RunVelocity : FlyVelocity : vel;

		Position += Forward * vertical * vel * deltaTime;
		Position += normalize(cross(Forward, Up)) * horizontal * vel * deltaTime;

		SetViewMatrix();
	}

	void Float(float deltaTime, bool up = true)
	{
		if (!CanMove)
			return;

		Position.y += (up ? FlyVelocity : -FlyVelocity) * deltaTime;

		SetViewMatrix();
	}

	void Rotate(float vertical, float horizontal)
	{
		// se multiplica por .1 ya que la posicion ya sea en x o y, no esta limitada
		// entre -1 a 1, por ejemplo, si es 50 la posicion vertical, automaticamente 
		// como en el main se especifica que el cursor regrese a 0, bajara de 50 a 0,
		// 50, 49, 48, 47, 46, 45, 44 ...
		// logrando un movimiento suave hasta 0
		// 50 * .1 = 5
		// 49 * .1 = 4.9
		// 48 * .1 = 4.8
		// ...
		// 0
		Pitch += vertical * 0.1f * Sensibilidad;
		Yaw += horizontal * 0.1f * Sensibilidad;

		if (Pitch > 89.0f)
			Pitch = 89.0f;

		if (Pitch < -89.0f)
			Pitch = -89.0f;

		SetDirection();
		SetViewMatrix();
	}

	void SetIsRun(bool run)
	{
		IsRun = run;
	}

	bool GetIsFly()
	{
		return IsFly;
	}

	void SetIsFly(bool fly)
	{
		IsFly = fly;
	}

	bool GetCanMove()
	{
		return CanMove;
	}

	void SetCanMove(bool canMove)
	{
		CanMove = canMove;
	}

	void VerificarColision(vec3 posicionModelo, vec3 colision)
	{
		if ((Position.x <= posicionModelo.x + colision.x && Position.x >= posicionModelo.x - colision.x)
			&& (Position.z <= posicionModelo.z + colision.z && Position.z >= posicionModelo.z - colision.z)
			&& (Position.y <= posicionModelo.y + colision.y && Position.y >= posicionModelo.y - colision.y))
		{ 
			std::cout << "Colision!" << std::endl; 
			SetPosition(AntesColisionPosition);
			SetViewMatrix();
		}

		AntesColisionPosition.x = Position.x;
		AntesColisionPosition.y = Position.y;
		AntesColisionPosition.z = Position.z;
	}

	void VerificarColision(vec3 posicionModelo, vec3 colision, float deltaTime)
	{
		if ((Position.x <= posicionModelo.x + colision.x && Position.x >= posicionModelo.x - colision.x)
			&& (Position.z <= posicionModelo.z + colision.z && Position.z >= posicionModelo.z - colision.z)
			&& (Position.y <= posicionModelo.y + colision.y && Position.y >= posicionModelo.y - colision.y))
		{
			std::cout << "Colision!" << std::endl;
			Move(-1, 0, deltaTime);
		}
	}

private:

	void SetViewMatrix()
	{
		ViewMatrix = lookAt(Position, Position + Forward, Up);
	}

	void SetDirection()
	{
		vec3 direction = vec3(0.0f);

		direction.x = cos(radians(Yaw)) * cos(radians(Pitch));
		direction.y = sin(radians(Pitch));
		direction.z = sin(radians(Yaw)) * cos(radians(Pitch));

		Forward = normalize(direction);
	}

};

#endif // !_CAMERA_
