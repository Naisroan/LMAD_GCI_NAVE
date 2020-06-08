#pragma once

#ifndef _FIRSTSCENE_
#define _FIRSTSCENE_

#include <stdlib.h>
#include <time.h>

#include <glfw3.h>
#include "Scene.h"
#include "Triangulo.h"
#include "Modelo.h"
#include "skybox.h"
#include "agua.h"
#include "clouds.h"
#include "terreno.h"
#include "Model.h"
#include "billboard.h"
#include "GamePadRR.h"

using namespace std;

class FirstScene 
	: public Scene
{

private:

	float escenaCargada = false;

	const int limiteX = 90;
	const int limiteZ = 90;

	float skyTime = 2.0f;
	bool toNight = true;

	vec3 lightPosition = vec3(18.0f, 26.0f, 130.0f);

	GamePadRR* joystick;

	Camera*	camara;
	Clouds*	m_clouds_plane;
	Skybox*	skybox;
	Terreno* terreno;
	Agua* agua;

	Shader* sh_roca;
	Model* m_roca;

	Shader* sh_arbol;
	Model* m_arbol;

	Shader* sh_casa;
	Model* m_casa;

	Shader* sh_nave;
	Model* m_nave;

	Shader* sh_bill;
	Billboard* bill_Arbusto;
	Billboard* bill_Arbusto2;
	Billboard* bill_Cesped;
	Billboard* bill_Flor;
	Billboard* bill_Montania;

	vec3 posRndmArboles[50] = { vec3(0.0f) };
	vec3 posRndmArbusto[75] = { vec3(0.0f) };
	vec3 posRndmArbusto2[10] = { vec3(0.0f) };
	vec3 posRndmCesped[75] = { vec3(0.0f) };
	vec3 posRndmFlores[15] = { vec3(0.0f) };

protected:

	virtual void Update()
	{
		Input();

		if (skyTime >= 2.0f)
		{
			toNight = true;
		}

		if (skyTime <= -0.25f)
		{
			toNight = false;
		}

		if (toNight)
		{
			skyTime -= 0.001f;
			SetClearColor(GetClearColor() + vec3(-0.0005f));
		}
		else
		{
			skyTime += 0.001f;
			SetClearColor(GetClearColor() + vec3(0.0005f));
		}

		// printf("Color del cielo: R: %f, G: %f, B: %f\n", GetClearColor().x, GetClearColor().y, GetClearColor().z);
	}

	virtual void Render()
	{
		mat4 view = camara->GetView();
		mat4 projection = GetProjection();
		vec3 skyColor = GetClearColor();

		DrawModels(camara, projection, skyColor, lightPosition);
		DrawBillBoards(camara, projection, skyColor, lightPosition);

		terreno->Draw(camara, projection, skyColor, lightPosition);
		agua->Draw(camara, projection, skyColor, lightPosition);
		skybox->Draw(view, projection, skyColor, skyTime, toNight, GetDeltaTime());
		m_clouds_plane->Draw(camara, projection, skyColor, lightPosition);
	}

public:

	FirstScene(GLFWwindow* window)
		: Scene(window)
	{
		srand(time(NULL));

		CargarCamara();
		CargarSky();
		CargarTerreno();
		CargarAgua();

		// tratar de cargar todo desde aqui pq necesito el terreno para calcular la altura en 'y' y en algunas cosas
		CargarPosicionesRandom();
		CargarModelos();
		CargarBillboards(terreno);

		joystick = new GamePadRR();
	}

	~FirstScene()
	{
		if (joystick)
			delete joystick;

		if (camara)
			delete camara;

		if (skybox)
			delete skybox;

		if (m_clouds_plane)
			delete m_clouds_plane;

		if (terreno)
			delete terreno;
		
		if (bill_Arbusto)
			delete bill_Arbusto;

		if (bill_Arbusto2)
			delete bill_Arbusto2;

		if (bill_Cesped)
			delete bill_Cesped;

		if (bill_Flor)
			delete bill_Flor;

		if (bill_Montania)
			delete bill_Montania;

		if (sh_casa)
			delete sh_casa;

		if (m_casa)
			delete m_casa;

		if (sh_roca)
			delete sh_roca;

		if (m_roca)
			delete m_roca;

		if (sh_arbol)
			delete sh_arbol;

		if (m_arbol)
			delete m_arbol;

		if (sh_nave)
			delete sh_nave;

		if (m_nave)
			delete m_nave;

		if (agua)
			delete agua;
	}

private:

	void DrawModels(Camera* camera, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		// --------------------------------------------------
		//	"Nave"
		// --------------------------------------------------
		sh_nave->use();
		sh_nave->setUniform("lightPosition", lightPosition);
		sh_nave->setUniform("lightColor", skyColor);
		sh_nave->setUniform("ambientCoe", 0.2f);
		sh_nave->setUniform("cameraPosition", camera->GetPosition());
		sh_nave->stopUsing();

		// m_nave->Draw(sh_nave, mat3(camera->GetView()), projection);
		m_nave->Draw(sh_nave, mat3(camera->GetView()), projection);

		// --------------------------------------------------
		//	"Casa"
		// --------------------------------------------------
		sh_casa->use();
		sh_casa->setUniform("lightPosition", lightPosition);
		sh_casa->setUniform("lightColor", skyColor);
		sh_casa->setUniform("ambientCoe", 0.1f);
		sh_casa->setUniform("cameraPosition", camera->GetPosition());
		sh_casa->stopUsing();

		m_casa->Draw(sh_casa, camera->GetView(), projection);

		// --------------------------------------------------
		//	"Arboles"
		// --------------------------------------------------
		sh_arbol->use();
		sh_arbol->setUniform("lightPosition", lightPosition);
		sh_arbol->setUniform("lightColor", skyColor);
		sh_arbol->setUniform("ambientCoe", 0.1f);
		sh_arbol->setUniform("cameraPosition", camera->GetPosition());
		sh_arbol->stopUsing();

		for (int i = 0; i < sizeof(posRndmArboles) / sizeof(vec3); i++)
		{
			m_arbol->SetPosition(posRndmArboles[i]);
			m_arbol->Draw(sh_arbol, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Montañas"
		// --------------------------------------------------
		sh_roca->use();
		sh_roca->setUniform("lightPosition", lightPosition);
		sh_roca->setUniform("lightColor", skyColor);
		sh_roca->setUniform("ambientCoe", 0.1f);
		sh_roca->setUniform("cameraPosition", camera->GetPosition());
		sh_roca->stopUsing();

		// parte delantera
		m_roca->SetPosition(vec3(-100.0f, 0.0f, -175.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetPosition(vec3(100.0f, 0.0f, -175.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetRotation(vec3(0.0f, 180.0f, 0.0f));
		m_roca->SetPosition(vec3(0.0f, 0.0f, -175.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		// parte derecha
		m_roca->SetRotation(vec3(0.0f, 0.0f, 0.0f));
		m_roca->SetPosition(vec3(175.0f, 0.0f, -100.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetPosition(vec3(175.0f, 0.0f, 100.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetRotation(vec3(0.0f, 180.0f, 0.0f));
		m_roca->SetPosition(vec3(175.0f, 0.0f, 0.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		// parte trasera
		m_roca->SetPosition(vec3(0.0f, 0.0f, 250.0f));
		m_roca->SetScale(vec3(3.0f, 2.0f, 2.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		// parte izquiera
		m_roca->SetScale(vec3(1.0f, 1.0f, 1.0f));
		m_roca->SetRotation(vec3(0.0f, 0.0f, 0.0f));
		m_roca->SetPosition(vec3(-175.0f, 0.0f, -100.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetPosition(vec3(-175.0f, 0.0f, 100.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

		m_roca->SetRotation(vec3(0.0f, 180.0f, 0.0f));
		m_roca->SetPosition(vec3(-175.0f, 0.0f, 0.0f));
		m_roca->Draw(sh_roca, camera->GetView(), projection);

	}

	void DrawBillBoards(Camera* camera, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		for (int i = 0; i < sizeof(posRndmArbusto) / sizeof(vec3); i++)
		{
			vec3 position = bill_Arbusto->GetPosition();
			position = posRndmArbusto[i];
			position.y += bill_Arbusto->size.y / 2.0f;

			bill_Arbusto->SetPosition(position);
			bill_Arbusto->Draw(camera, projection, skyColor, lightPosition);
		}

		for (int i = 0; i < sizeof(posRndmArbusto2) / sizeof(vec3); i++)
		{
			vec3 position = bill_Arbusto2->GetPosition();
			position = posRndmArbusto2[i];
			position.y += bill_Arbusto2->size.y / 2.0f;

			bill_Arbusto2->SetPosition(position);
			bill_Arbusto2->Draw(camera, projection, skyColor, lightPosition);
		}

		for (int i = 0; i < sizeof(posRndmCesped) / sizeof(vec3); i++)
		{
			vec3 position = bill_Cesped->GetPosition();
			position = posRndmCesped[i];
			position.y += bill_Cesped->size.y / 2.0f;

			bill_Cesped->SetPosition(position);
			bill_Cesped->Draw(camera, projection, skyColor, lightPosition);
		}

		for (int i = 0; i < sizeof(posRndmFlores) / sizeof(vec3); i++)
		{
			vec3 position = bill_Flor->GetPosition();
			position = posRndmFlores[i];
			position.y += bill_Flor->size.y / 2.0f;

			bill_Flor->SetPosition(position);
			bill_Flor->Draw(camera, projection, skyColor, lightPosition);
		}

		vec3 position = vec3(0.0f, 0.0f, 0.0f);
		position.y += (bill_Montania->size.y / 2.0f) + 50.0f;

		position = vec3(0.1f, position.y, -200.0f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);

		position = vec3(-200.0f, position.y, 0.1f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);

		position = vec3(200.0f, position.y, 0.1f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);
	}

private:

	void CargarCamara()
	{
		camara = new Camera;
		camara->SetPosition(vec3(25.0f, 20.0f, 30.0f));
	}

	void CargarSky()
	{
		// orden right, left, top, bottom, back, front
		skybox = new Skybox(vector<string> {
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_lf.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_rt.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_up.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_dn.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_ft.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/bluecloud_bk.jpg")
		},
		vector<string>{
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_lf.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_rt.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_up.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_dn.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_ft.jpg"),
			Fun::GetCarpetaRecursos("skybox/clouds/yellowcloud_bk.jpg")
		},
		vector<string> {
			Fun::GetCarpetaRecursos("skybox/noche/noche_left.png"),
			Fun::GetCarpetaRecursos("skybox/noche/noche_right.png"),
			Fun::GetCarpetaRecursos("skybox/noche/noche_up.png"),
			Fun::GetCarpetaRecursos("skybox/noche/noche_down.png"),
			Fun::GetCarpetaRecursos("skybox/noche/noche_front.png"),
			Fun::GetCarpetaRecursos("skybox/noche/noche_back.png")
		}, Fun::GetCarpetaRecursos("skybox/skybox.vert"), Fun::GetCarpetaRecursos("skybox/skybox.frag"));

		m_clouds_plane = new Clouds(Fun::GetCarpetaRecursos("skybox/clouds/clouds_plane.obj"), Fun::GetCarpetaRecursos("skybox/clouds/clouds_plane.png"),
			Fun::GetCarpetaRecursos("skybox/clouds.vert"), Fun::GetCarpetaRecursos("skybox/clouds.frag"));

		m_clouds_plane->SetPosition(vec3(0.0f, 100.0f, 0.0f));
	}

	void CargarTerreno()
	{
		terreno = new Terreno(Fun::GetCarpetaRecursos("terreno/terrenoMap5.png"),
			Fun::GetCarpetaRecursos("terreno/v01/grass01v2.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass01_n.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass02.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass02_n.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/multi_tex.jpg"),
			Fun::GetCarpetaRecursos("shaders/terreno.vert"),
			Fun::GetCarpetaRecursos("shaders/terreno.frag"));

		terreno->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	}

	void CargarAgua()
	{
		agua = new Agua(Fun::GetCarpetaRecursos("agua/agua.png"),
			Fun::GetCarpetaRecursos("agua/aguahm.jpg"),
			Fun::GetCarpetaRecursos("agua/agua_specular.jpg"),
			Fun::GetCarpetaRecursos("shaders/agua.vert"),
			Fun::GetCarpetaRecursos("shaders/agua.frag"));

		agua->SetPosition(vec3(310.0f, 0.3f, 0.0f));
	}

	void CargarModelos()
	{
		vector<ShaderFile> shaderFiles;

		// --------------------------------------------------
		//	"Nave"
		// --------------------------------------------------

		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/nave.vert"), GL_VERTEX_SHADER));
		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/nave.frag"), GL_FRAGMENT_SHADER));
		sh_nave = new Shader(shaderFiles);
		shaderFiles.clear();

		m_nave = Model::ObjToModel(sh_nave, Fun::GetCarpetaRecursos("models/nave"), "nave.obj");
		m_nave->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		m_nave->SetScale(vec3(0.7f, 0.7f, 0.7f));

		// --------------------------------------------------
		//	"Casa"
		// --------------------------------------------------

		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/casa.vert"), GL_VERTEX_SHADER));
		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/casa.frag"), GL_FRAGMENT_SHADER));
		sh_casa = new Shader(shaderFiles);
		shaderFiles.clear();

		m_casa = Model::ObjToModel(sh_casa, Fun::GetCarpetaRecursos("models/casa"), "casa.obj");

		m_casa->SetRotation(vec3(0.0f, 160.0f, 0.0f));
		m_casa->SetPosition(vec3(4.0f, 0.0f, -65.0f));
		m_casa->SetPosition(vec3(m_casa->GetPosition().x, terreno->GetHeightFromPoint(m_casa->GetPosition()), m_casa->GetPosition().z));

		// --------------------------------------------------
		//	"Roca"
		// --------------------------------------------------

		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/model.vert"), GL_VERTEX_SHADER));
		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/model.frag"), GL_FRAGMENT_SHADER));
		sh_roca = new Shader(shaderFiles);
		shaderFiles.clear();

		m_roca = Model::ObjToModel(sh_roca, Fun::GetCarpetaRecursos("models/roca"), "Stone_Pack1_Stone_1.obj");

		// --------------------------------------------------
		//	"Arboles"
		// --------------------------------------------------

		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/tree02.vert"), GL_VERTEX_SHADER));
		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/tree02.frag"), GL_FRAGMENT_SHADER));
		sh_arbol = new Shader(shaderFiles);
		shaderFiles.clear();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_arbol = Model::ObjToModel(sh_arbol, Fun::GetCarpetaRecursos("models/tree02"), "Tree.obj");

		glDisable(GL_BLEND);

		m_arbol->SetPosition(vec3(0.0f, 0.0f, 0.0f));
		m_arbol->SetPosition(vec3(m_arbol->GetPosition().x, terreno->GetHeightFromPoint(m_arbol->GetPosition()), m_arbol->GetPosition().z));
	}

	void CargarBillboards(Terreno* _terreno)
	{
		vector<ShaderFile> shaderFiles;

		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/bill.vert"), GL_VERTEX_SHADER));
		shaderFiles.push_back(ShaderFile::LoadShaderFromFile(Fun::GetCarpetaRecursos("shaders/bill.frag"), GL_FRAGMENT_SHADER));
		sh_bill = new Shader(shaderFiles);
		shaderFiles.clear();

		bill_Arbusto = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/arbol.png"), vec2(1.0f, 1.0f));
		bill_Arbusto2 = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/arbol2.png"), vec2(3.4f, 3.0f));
		bill_Cesped = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/grass01.png"), vec2(1.0f, 1.0f));
		bill_Flor = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/roca.png"), vec2(1.0f, 1.0f));
		bill_Montania = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/mountain.png"), vec2(255.0f, 100.0f));
	}

	void CargarPosicionesRandom()
	{
		for (int i = 0; i < sizeof(posRndmArboles) / sizeof(vec3); i++)
		{
			int rndmX = -40 + rand() % (40 + 1 - (-40));
			int rndmZ = -40 + rand() % (40 + 1 - (-40));

			posRndmArboles[i] = vec3(rndmX, 0.0f, rndmZ);
			posRndmArboles[i].y = terreno->GetHeightFromPoint(posRndmArboles[i]);
		}

		for (int i = 0; i < sizeof(posRndmArbusto) / sizeof(vec3); i++)
		{
			int rndmX = -40 + rand() % (25 + 1 - (-40));
			int rndmZ = -40 + rand() % (25 + 1 - (-40));

			posRndmArbusto[i] = vec3(rndmX, 0.0f, rndmZ);
			posRndmArbusto[i].y = terreno->GetHeightFromPoint(posRndmArbusto[i]);
		}

		for (int i = 0; i < sizeof(posRndmArbusto2) / sizeof(vec3); i++)
		{
			int rndmX = -40 + rand() % (25 + 1 - (-40));
			int rndmZ = -40 + rand() % (25 + 1 - (-40));

			posRndmArbusto2[i] = vec3(rndmX, 0.0f, rndmZ);
			posRndmArbusto2[i].y = terreno->GetHeightFromPoint(posRndmArbusto2[i]);
		}

		for (int i = 0; i < sizeof(posRndmCesped) / sizeof(vec3); i++)
		{
			int rndmX = -25 + rand() % (25 + 1 - (-25));
			int rndmZ = -25 + rand() % (25 + 1 - (-25));

			posRndmCesped[i] = vec3(rndmX, 0.0f, rndmZ);
			posRndmCesped[i].y = terreno->GetHeightFromPoint(posRndmCesped[i]);
		}

		for (int i = 0; i < sizeof(posRndmFlores) / sizeof(vec3); i++)
		{
			int rndmX = -25 + rand() % (25 + 1 - (-25));
			int rndmZ = -25 + rand() % (25 + 1 - (-25));

			posRndmFlores[i] = vec3(rndmX, 0.0f, rndmZ);
			posRndmFlores[i].y = terreno->GetHeightFromPoint(posRndmFlores[i]);
		}
	}

private:

	// funcion que hice que me puede ser util en el futuro
	void TimeLineRotarTriangulo(float valorInicial, float valorFinal, float segInicial,  float segFinal, float& salida)
	{
		static float segsElapsed = 0.0f;
		float deltaTime = GetDeltaTime();

		deltaTime = deltaTime <= 0.0f ? 0.0f : deltaTime;

		if (segsElapsed >= segInicial && segsElapsed <= segFinal)
		{
			salida += (valorInicial + ((valorFinal / segFinal) * deltaTime));

			if (salida > valorFinal)
			{
				salida = valorFinal;
			}

			cout << "Segundos: " << segsElapsed << " Salida: " << salida << endl;


			segsElapsed += deltaTime <= 0.0f ? 0 : deltaTime;
		}
	}

	void Input()
	{
		static GLFWwindow* gameWindow = GetGameWindow();

		// cerrar ventana
		if (glfwGetKey(gameWindow, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(gameWindow, 1);
		}

		if (joystick->BotonPresionado(GamePadRR::Boton::BACK))
		{
			glfwSetWindowShouldClose(gameWindow, 1);
		}
		
		InputColisiones(gameWindow);
		InputMoveDirectionCamara(gameWindow);
		InputGiroJoystick();

		vec3 naveRotation = m_nave->GetRotation();
		m_nave->SetRotation(vec3(naveRotation.x, -camara->GetYaw() + 90.0f, naveRotation.z));

		InputLightPosition(gameWindow, true);
		InputLightColor(gameWindow, true);
	}

	void InputColisiones(GLFWwindow* gameWindow)
	{
		vec3 cameraPosition = camara->GetPosition();
		float radioNave = 3.0f;

		// --------------------------------------------------
		//	"Terreno"
		// --------------------------------------------------
		if (cameraPosition.x <= -limiteX || cameraPosition.x >= limiteX)
		{
			cameraPosition.x = cameraPosition.x < 0.0f ? -limiteX + 0.1 : limiteX - 0.01;
			camara->SetPosition(cameraPosition);
		}

		if (cameraPosition.z <= -limiteZ || cameraPosition.z >= limiteZ)
		{
			cameraPosition.z = cameraPosition.z < 0.0f ? -limiteZ + 0.1 : limiteZ - 0.01;
			camara->SetPosition(cameraPosition);
		}

		float posicionRespectoTerreno = terreno->GetHeightFromPoint(cameraPosition);

		// el 3.0f es considerando la nave
		if (cameraPosition.y - 3.0f < posicionRespectoTerreno && camara->GetIsFly())
		{
			cameraPosition.y = posicionRespectoTerreno + 3.01f;
			camara->SetPosition(cameraPosition);
		}

		// --------------------------------------------------
		//	"Arboles"
		// --------------------------------------------------
		for (vec3 posicionArbol : posRndmArboles)
		{
			float radio = 2.0f;
			vec3 center = posicionArbol + vec3(0.0f, radio + 1.0, 0.0f);

			// obtenemos distancia _/(x2 - x1)^2 + (z2 - z1)^2
			float distance = glm::distance(cameraPosition, center);

			if (distance <= radio)
			{
				camara->Move(-1.0f, 0.0f, GetDeltaTime());
			}
		}

		// --------------------------------------------------
		//	"Casa"
		// --------------------------------------------------

		vec3 posicionCasa = m_casa->GetPosition();

		// si entra al if, significa que esta en area de la casa
		if (cameraPosition.z <= -45.0f && cameraPosition.z >= -85.0f
			&& cameraPosition.x >= -20.0f && cameraPosition.x <= 22.0f
			&& cameraPosition.y - 3.0f >= 0.0f && cameraPosition.y - 3.0f <= 25.0f)
		{
			// por arriba
			if (cameraPosition.y - 3.0f <= 24.0f)
			{
				cameraPosition.y += 0.1;
				camara->SetPosition(cameraPosition);
			}

			// intento de entrar por z hacia adelante
			if (cameraPosition.z <= -45.0f && cameraPosition.z >= -60.0f)
			{
				cameraPosition.z += 0.1;
				camara->SetPosition(cameraPosition);
			}

			// intento de entrar por z por atras
			if (cameraPosition.z <= -70.0f && cameraPosition.z >= -90.0f)
			{
				cameraPosition.z -= 0.1;
				camara->SetPosition(cameraPosition);
			}

			// intento de entrar por x por izquierda
			if (cameraPosition.x >= -20.0f && cameraPosition.x <= -15.0f)
			{
				cameraPosition.x -= 0.1;
				camara->SetPosition(cameraPosition);
			}

			// intento de entrar por x por derecha
			if (cameraPosition.x <= 22.0f && cameraPosition.x >= 17.0f)
			{
				cameraPosition.x += 0.1;
				camara->SetPosition(cameraPosition);
			}
		}

 		printf("X: %f, Y: %f, Z: %f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

		InputMoveJoystick();
		InputMoveCamara(gameWindow);
	}

	void InputMoveJoystick()
	{
		if (!joystick->EstaConectado())
		{
			return;
		}

		float movimientoVertical = joystick->GetStickLeftVertical();
		float movimientoHorizontal = joystick->GetStickLeftHorizontal();

		camara->Move(movimientoVertical, 0.0f, GetDeltaTime());
		camara->Move(0.0f, movimientoHorizontal, GetDeltaTime());

		if (camara->GetIsFly())
		{
			if (joystick->BotonPresionado(GamePadRR::Boton::RT))
			{
				camara->Float(GetDeltaTime());
			}

			if (joystick->BotonPresionado(GamePadRR::Boton::LT))
			{
				camara->Float(GetDeltaTime(), false);
			}
		}
	}

	void InputGiroJoystick()
	{
		if (!joystick->EstaConectado())
		{
			return;
		}

		float GiroVertical = joystick->GetStickRightVertical();
		float GiroHorizontal = joystick->GetStickRightHorizontal();

		camara->Rotate(GiroVertical * 3.0f, GiroHorizontal * 3.0f);
	}

	void InputMoveCamara(GLFWwindow* gameWindow)
	{
		if (glfwGetKey(gameWindow, GLFW_KEY_LEFT_SHIFT))
		{
			camara->SetIsRun(true);
		}
		else
		{
			camara->SetIsRun(false);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_W))
		{
			camara->Move(1.0f, 0.0f, GetDeltaTime());
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_D))
		{
			camara->Move(0.0f, 1.0f, GetDeltaTime());
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_S))
		{
			camara->Move(-1.0f, 0.0f, GetDeltaTime());
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_A))
		{
			camara->Move(0.0f, -1.0f, GetDeltaTime());
		}

		// activamos la opcion de volaaar
		/*if (glfwGetKey(gameWindow, GLFW_KEY_Q))
		{
			camara->SetIsFly(!camara->GetIsFly());
		}*/

		if (camara->GetIsFly() && glfwGetKey(gameWindow, GLFW_KEY_SPACE))
		{
			camara->Float(GetDeltaTime());
		}

		// si no esta volando... establecemos que la camara se quede pegada al piso
		//if (!camara->GetIsFly())
		//{
		//	vec3 posCamara = camara->GetPosition();

		//	// le sumamos mas 1.7 para que no quede totalmente pegado al piso (simulamos que mide 1.7 mts)
		//	posCamara.y = terreno->GetHeightFromPoint(camara->GetPosition()) + 1.7f;
		//	camara->SetPosition(posCamara);
		//}

		// m_nave->SetPosition(camara->GetPosition() + vec3(0.0f, -5.0f, -5.0f));
	}

	void InputMoveDirectionCamara(GLFWwindow* gameWindow)
	{
		// direccion con mouse
		double mouseX, mouseY;

		glfwGetCursorPos(gameWindow, &mouseX, &mouseY);

		camara->Rotate((float)-mouseY, (float)mouseX);
	}

	void InputLightPosition(GLFWwindow* gameWindow, bool show = false)
	{
		if (glfwGetKey(gameWindow, GLFW_KEY_I))
		{
			lightPosition += vec3(0.0f, 0.0f, 1.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_L))
		{
			lightPosition += vec3(1.0f, 0.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_K))
		{
			lightPosition += vec3(0.0f, 0.0f, -1.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_J))
		{
			lightPosition += vec3(-1.0f, 0.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_U))
		{
			lightPosition += vec3(0.0f, 1.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_O))
		{
			lightPosition += vec3(0.0f, -1.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}
	}

	void InputLightColor(GLFWwindow* gameWindow, bool show = false)
	{
		if (glfwGetKey(gameWindow, GLFW_KEY_UP))
		{
			vec3 upColor = GetClearColor() + vec3(0.01, 0.01, 0.01);

			// ponemos como limite que no se pase de 1.0 en adelante en cualquier color
			upColor = glm::min(upColor, vec3(1.0, 1.0, 1.0));

			SetClearColor(upColor);

			if (show)
				printf_s("SkyColor(%f, %f, %f)\n", upColor.x, upColor.y, upColor.z);
		}

		if (glfwGetKey(gameWindow, GLFW_KEY_DOWN))
		{
			vec3 downColor = GetClearColor() - vec3(0.01, 0.01, 0.01);

			// ponemos como limite que no se pase de 0.0 hacia atras en cualquier color
			downColor = glm::max(downColor, vec3(0.0, 0.0, 0.0));

			SetClearColor(downColor);

			if (show)
				printf_s("SkyColor(%f, %f, %f)\n", downColor.x, downColor.y, downColor.z);
		}
	}
};

#endif // !_FIRSTSCENE_