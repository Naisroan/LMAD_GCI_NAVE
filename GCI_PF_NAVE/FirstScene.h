#pragma once

#ifndef _FIRSTSCENE_
#define _FIRSTSCENE_

#include <stdlib.h>
#include <time.h>

#include "Scene.h"
#include "terreno.h"
#include "Camera.h"
#include "GamePadRR.h"
#include "skybox.h"
#include "agua.h"
#include "billboard.h"
#include "Model.h"

using namespace std;

class FirstScene 
	: public Scene
{

private:

	// limites del terreno
	const int limiteX = 90;
	const int limiteZ = 90;

	// para fines de pruebas (si se pone false no lo carga y por ende no lo dibuja)
	const bool CICLO_DIA_NOCHE = true;
	const bool CARGA_DIBUJA_SOL = true;
	const bool CARGA_DIBUJA_SOL_BILL = false;
	const bool CARGA_DIBUJA_NAVE = true;
	const bool CARGA_DIBUJA_ARBOL = true;
	const bool CARGA_DIBUJA_CASA = true;
	const bool CARGA_DIBUJA_BOTE = true;
	const bool CARGA_DIBUJA_ROCAS = true;
	const bool CARGA_DIBUJA_ROCAS2 = true;
	const bool CARGA_DIBUJA_LUCIERNAGAS = false;

	float skyTime = 2.5f;
	bool toNight = true;

	// vec3 lightPosition = vec3(170.0f, 160.0f, 165.0f);
	vec3 lightPosition = vec3(0.0f, 0.0f, 0.0f);

	GamePadRR* joystick;

	Camera*	camara;
	Terreno* terreno;
	Skybox* sky;
	Agua* agua;

	Shader* sh_nave;
	Model* m_nave;

	Shader* sh_casa;
	Model* m_casa;
	Model* m_arbol_mochado;

	Shader* sh_model;
	Model* m_roca;
	Model* m_roca2;
	Model* m_boat;
	Model* m_hacha;

	Shader* sh_arbol;
	Model* m_arbol;
	Model* m_arbol2;

	Shader* sh_sol;
	Model* m_sol;

	Shader* sh_bill;
	Billboard* bill_sol;
	Billboard* bill_Arbusto;
	Billboard* bill_Arbusto2;
	Billboard* bill_Cesped;
	Billboard* bill_Flor;
	Billboard* bill_Montania;

	int tipoRndmArboles[50] = { 0 };
	vec3 posRndmArboles[sizeof(tipoRndmArboles) / sizeof(int)] = { vec3(0.0f) };
	vec3 posRndmArbusto[75] = { vec3(0.0f) };
	vec3 posRndmArbusto2[10] = { vec3(0.0f) };
	vec3 posRndmCesped[75] = { vec3(0.0f) };
	vec3 posRndmFlores[15] = { vec3(0.0f) };
	vec3 posRndmLuciernagas[5] = { vec3(0.0f) };

public:

	FirstScene(HWND* window)
		: Scene(window)
	{
		srand(time(NULL));

		CargarTerreno();
		CargarCamara();
		CargarSky();
		CargarAgua();
		CargarPosiciones();
		CargarBillboards();
		CargarModelos();

		// tratar de cargar todo desde aqui pq necesito el terreno para calcular la altura en 'y' y en algunas cosas
		joystick = new GamePadRR();
	}

	~FirstScene()
	{
		if (joystick)
			delete joystick;

		if (camara)
			delete camara;

		if (terreno)
			delete terreno;

		if (sky)
			delete sky;

		if (agua)
			delete agua;

		if (bill_sol)
			delete bill_sol;

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

		if (sh_nave)
			delete sh_nave;

		if (m_nave)
			delete m_nave;

		if (sh_model)
			delete sh_model;

		if (sh_casa)
			delete sh_casa;

		if (m_casa)
			delete m_casa;

		if (m_boat)
			delete m_boat;

		if (sh_arbol)
			delete sh_arbol;

		if (m_arbol)
			delete m_arbol;

		if (m_arbol2)
			delete m_arbol2;

		if (m_roca)
			delete m_roca;

		if (m_roca2)
			delete m_roca2;

		if (m_arbol_mochado)
			delete m_arbol_mochado;

		if (m_hacha)
			delete m_hacha;

		if (sh_sol)
			delete sh_sol;

		if (m_sol)
			delete m_sol;
	}

protected:

	virtual void Update()
	{
		Input();

		if (CICLO_DIA_NOCHE)
		{
			if (skyTime >= 2.8f)
			{
				toNight = true;
			}

			if (skyTime <= -0.5f)
			{
				toNight = false;
			}

			if (toNight)
			{
				skyTime -= 0.001f;
			}
			else
			{
				skyTime += 0.001f;
			}

			SetSkyColor(skyTime, toNight);

			// printf("CIELO: R: %f, G: %f, B: %f\n", skyColor.x, skyColor.y, skyColor.z);
		}

		if (CARGA_DIBUJA_SOL && CICLO_DIA_NOCHE)
		{
			static float rotacionSol = -45.0f;
			static float radio = 160.0f;

			rotacionSol += 0.035;

			// regresar sol al "horizonte"
			if (rotacionSol >= 70.0f && skyTime <= 0.5f)
			{
				rotacionSol = -145.0f;
			}

			// formula de rotacion: x = xcos0 - ysin0, z = ycos0 + xsen0
			lightPosition.y = (radio * cosf(radians(rotacionSol))) - (radio * sinf(radians(rotacionSol)));
			lightPosition.z = (radio * cosf(radians(rotacionSol))) + (radio * sinf(radians(rotacionSol)));

			m_sol->SetPosition(lightPosition);
			// cout << rotacionSol << endl;
		}
	}

	virtual void Render()
	{
		mat4 view = camara->GetView();
		mat4 projection = GetProjection();

		terreno->Draw(camara, projection, skyColor, lightPosition);
		sky->Draw(view, projection, skyColor, skyTime, toNight, GetDeltaTime());
		agua->Draw(camara, projection, skyColor, lightPosition);

		DrawBillBoards(camara, projection, skyColor, lightPosition);
		DrawModels(camara, projection, skyColor, lightPosition);
	}

private:

	void CargarTerreno()
	{
		terreno = new Terreno(Fun::GetCarpetaRecursos("terreno/terrenoMap8.png"),
			Fun::GetCarpetaRecursos("terreno/v01/grass01v2.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass01_n.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass02.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/grass02_n.jpg"),
			Fun::GetCarpetaRecursos("terreno/v01/multi_tex.jpg"),
			"shaders/terreno.vert",
			"shaders/terreno.frag");

		terreno->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	}

	void CargarCamara()
	{
		camara = new Camera;
		camara->SetPosition(vec3(-50.0f, 25.0f, 50.0f));
	}

	void CargarSky()
	{
		sky = new Skybox(vector<string> {
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
		}, 
		"skybox/skybox.vert", 
		"skybox/skybox.frag");
	}

	void CargarAgua()
	{
		agua = new Agua(Fun::GetCarpetaRecursos("agua/agua.png"),
			Fun::GetCarpetaRecursos("agua/aguahm.jpg"),
			Fun::GetCarpetaRecursos("agua/agua_n.jpg"),
			Fun::GetCarpetaRecursos("agua/agua_s.jpg"),
			"shaders/agua.vert",
			"shaders/agua.frag");

		agua->SetPosition(vec3(180.0f, 2.0f, 0.0f));
	}

	void CargarPosiciones()
	{
		if (CARGA_DIBUJA_ARBOL)
		{
			for (int i = 0; i < sizeof(posRndmArboles) / sizeof(vec3); i++)
			{
				int rndmX = -70 + rand() % (50 + 1 - (-70));
				int rndmZ = -40 + rand() % (70 + 1 - (-40));
				int rndmTipo = 1 + rand() % (2 + 1 - (1));

				posRndmArboles[i] = vec3(rndmX, 0.0f, rndmZ);
				posRndmArboles[i].y = terreno->GetHeightFromPoint(posRndmArboles[i]);
				tipoRndmArboles[i] = rndmTipo;
			}
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

		if (CARGA_DIBUJA_LUCIERNAGAS)
		{
			for (int i = 0; i < sizeof(posRndmLuciernagas) / sizeof(vec3); i++)
			{
				int rndmX = -25 + rand() % (25 + 1 - (-25));
				int rndmZ = -25 + rand() % (25 + 1 - (-25));
				int rndmY = 0 + rand() % (7 + 1 - (0));

				posRndmLuciernagas[i] = vec3(rndmX, 0.0f, rndmZ);
				posRndmLuciernagas[i].y = terreno->GetHeightFromPoint(posRndmFlores[i]) + rndmY;
			}
		}
	}

	void CargarBillboards()
	{
		sh_bill = new Shader("shaders/bill.vert", "shaders/bill.frag");

		bill_sol = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/sol.png"), vec2(15.0f, 15.0f));
		bill_Arbusto = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/arbol.png"), vec2(1.0f, 1.0f));
		bill_Arbusto2 = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/arbol2.png"), vec2(3.4f, 3.0f));
		bill_Cesped = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/grass01.png"), vec2(1.0f, 1.0f));
		bill_Flor = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/roca.png"), vec2(1.0f, 1.0f));
		bill_Montania = new Billboard(sh_bill, Fun::GetCarpetaRecursos("bills/mountain.png"), vec2(400.0f, 200.0f));
	}

	void CargarModelos()
	{
		// --------------------------------------------------
		//	"Shader default"
		// --------------------------------------------------
		sh_model = new Shader("shaders/model.vert", "shaders/model.frag");

		// --------------------------------------------------
		//	"Sol"
		// --------------------------------------------------

		if (CARGA_DIBUJA_SOL)
		{
			sh_sol = new Shader("shaders/sol.vert", "shaders/sol.frag");

			m_sol = Model::CargarOBJ(sh_sol, Fun::GetCarpetaRecursos("models/sun"), "sol_luna.obj");
			m_sol->SetPosition(lightPosition);
		}

		// --------------------------------------------------
		//	"Nave"
		// --------------------------------------------------

		if (CARGA_DIBUJA_NAVE)
		{
			sh_nave = new Shader("shaders/nave.vert", "shaders/nave.frag");

			m_nave = Model::CargarOBJ(sh_nave, Fun::GetCarpetaRecursos("models/nave"), "nave.obj");
			m_nave->SetRotation(vec3(0.0f, 180.0f, 0.0f));
			m_nave->SetScale(vec3(0.1f, 0.1f, 0.1f));
		}
		

		// --------------------------------------------------
		//	"Arbol"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ARBOL)
		{
			sh_arbol = new Shader("shaders/tree02.vert", "shaders/tree02.frag");

			m_arbol = Model::CargarOBJ(sh_arbol, Fun::GetCarpetaRecursos("models/tree02"), "Tree.obj");
			m_arbol->SetScale(vec3(3.0f));

			m_arbol2 = Model::CargarOBJ(sh_arbol, Fun::GetCarpetaRecursos("models/tree01"), "tree.obj");
			m_arbol2->SetScale(vec3(3.0f));
		}

		// --------------------------------------------------
		//	"CASA"
		// --------------------------------------------------

		if (CARGA_DIBUJA_CASA)
		{
			sh_casa = new Shader("shaders/casa.vert", "shaders/casa.frag");

			m_casa = Model::CargarOBJ(sh_casa, Fun::GetCarpetaRecursos("models/casa2"), "house_v2.obj");
			m_casa->SetPosition(vec3(15.0f, 3.8f, -75.0f));
			m_casa->SetRotation(vec3(0.0, 90.0f, 0.0f));
			m_casa->SetScale(vec3(0.3f));
		}

		// --------------------------------------------------
		//	"HACHA Y ARBOL MOCHADO"
		// --------------------------------------------------

		if (CARGA_DIBUJA_CASA)
		{
			m_arbol_mochado = Model::CargarOBJ(sh_casa, Fun::GetCarpetaRecursos("models/tree03"), "arbol_mochado.obj");
			m_hacha = Model::CargarOBJ(sh_casa, Fun::GetCarpetaRecursos("models/axe"), "axe.obj");

			vec3 posicion = vec3(35.0f, 5.0f, -45.0f);

			m_hacha->SetPosition(posicion);
			m_arbol_mochado->SetPosition(posicion);
		}

		// --------------------------------------------------
		//	"Bote"
		// --------------------------------------------------

		if (CARGA_DIBUJA_BOTE)
		{
			m_boat = Model::CargarOBJ(sh_model, Fun::GetCarpetaRecursos("models/boat"), "boat.obj");
			m_boat->SetPosition(vec3(75.0f, 5.0f, -45.0f));
		}

		// --------------------------------------------------
		//	"ROCAS"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ROCAS)
		{
			m_roca = Model::CargarOBJ(sh_model, Fun::GetCarpetaRecursos("models/roca"), "Stone_Pack1_Stone_1.obj");
		}

		// --------------------------------------------------
		//	"ROCAS 2"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ROCAS2)
		{
			m_roca2 = Model::CargarOBJ(sh_model, Fun::GetCarpetaRecursos("models/roca2"), "roca2.obj");
		}
	}

private:

	void DrawModels(Camera* camera, mat4 projection, vec3 skyColor, vec3 lightPosition)
	{
		// --------------------------------------------------
		//	"Sol"
		// --------------------------------------------------

		if (CARGA_DIBUJA_SOL)
		{
			sh_sol->use();
			sh_sol->setUniform("lightPos", lightPosition);
			sh_sol->setUniform("skyColor", skyColor);
			sh_sol->setUniform("viewPos", camera->GetPosition());
			sh_sol->stopUsing();

			// m_sol->SetPosition(lightPosition);
			// m_sol->Draw(sh_sol, mat3(camera->GetView()), projection);
			m_sol->Draw(sh_sol, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Nave"
		// --------------------------------------------------
		
		if (CARGA_DIBUJA_NAVE)
		{
			sh_nave->use();
			sh_nave->setUniform("lightPos", lightPosition);
			sh_nave->setUniform("skyColor", skyColor);
			sh_nave->setUniform("viewPos", camera->GetPosition());
			sh_nave->stopUsing();

			m_nave->SetPosition(camera->GetPosition());
			m_nave->SetRotation(vec3(0.0f, -camera->GetYaw() + 90.0f, 0.0f));
			m_nave->Draw(sh_nave, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Arbol"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ARBOL)
		{
			sh_arbol->use();
			sh_arbol->setUniform("lightPos", lightPosition);
			sh_arbol->setUniform("skyColor", skyColor);
			sh_arbol->setUniform("viewPos", camera->GetPosition());
			sh_arbol->stopUsing();

			for (int i = 0; i < sizeof(posRndmArboles) / sizeof(vec3); i++)
			{
				if (tipoRndmArboles[i] == 1)
				{
					m_arbol->SetPosition(posRndmArboles[i]);
					m_arbol->Draw(sh_arbol, camera->GetView(), projection);
				}
				else
				{
					m_arbol2->SetPosition(posRndmArboles[i]);
					m_arbol2->Draw(sh_arbol, camera->GetView(), projection);
				}
			}
		}

		// --------------------------------------------------
		//	"Casa"
		// --------------------------------------------------

		if (CARGA_DIBUJA_CASA)
		{
			sh_casa->use();
			sh_casa->setUniform("lightPos", lightPosition);
			sh_casa->setUniform("skyColor", skyColor);
			sh_casa->setUniform("viewPos", camera->GetPosition());
			sh_casa->stopUsing();

			m_casa->Draw(sh_casa, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Hacha y arbol mochado"
		// --------------------------------------------------

		if (CARGA_DIBUJA_CASA)
		{
			sh_casa->use();
			sh_casa->setUniform("lightPos", lightPosition);
			sh_casa->setUniform("skyColor", skyColor);
			sh_casa->setUniform("viewPos", camera->GetPosition());
			sh_casa->stopUsing();

			m_arbol_mochado->Draw(sh_casa, camera->GetView(), projection);
			m_hacha->Draw(sh_casa, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Bote"
		// --------------------------------------------------

		if (CARGA_DIBUJA_BOTE)
		{
			sh_model->use();
			sh_model->setUniform("lightPos", lightPosition);
			sh_model->setUniform("skyColor", skyColor);
			sh_model->setUniform("viewPos", camera->GetPosition());
			sh_model->setUniform("shininess", 10.0f);
			sh_model->stopUsing();

			m_boat->SetPosition(vec3(75.0f, 4.8f + agua->MovementVertex, -48.0f + -agua->MovementVertex));
			m_boat->Draw(sh_model, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Rocas"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ROCAS)
		{
			sh_model->use();
			sh_model->setUniform("lightPos", lightPosition);
			sh_model->setUniform("skyColor", skyColor);
			sh_model->setUniform("viewPos", camera->GetPosition());
			sh_model->setUniform("shininess", 32.0f);
			sh_model->stopUsing();

			m_roca->SetScale(vec3(0.3f));

			m_roca->SetRotation(vec3(0.0f, 35.0f, 0.0f));
			m_roca->SetPosition(vec3(72.0f, 3.0f, -18.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);

			m_roca->SetPosition(vec3(72.0f, 3.0f, -89.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);

			m_roca->SetRotation(vec3(0.0f));
			m_roca->SetPosition(vec3(56.0f, 0.0f, -118.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);

			m_roca->SetScale(vec3(0.6f));
			m_roca->SetRotation(vec3(0.0f));

			m_roca->SetPosition(vec3(0.0f, 0.0f, -160.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);

			m_roca->SetScale(vec3(1.0f));

			m_roca->SetPosition(vec3(120.0f, 0.0f, -180.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);

			m_roca->SetPosition(vec3(-160.0f, 0.0f, -160.0f));
			m_roca->Draw(sh_model, camera->GetView(), projection);
		}

		// --------------------------------------------------
		//	"Rocas 2"
		// --------------------------------------------------

		if (CARGA_DIBUJA_ROCAS2)
		{
			sh_model->use();
			sh_model->setUniform("lightPos", lightPosition);
			sh_model->setUniform("skyColor", skyColor);
			sh_model->setUniform("viewPos", camera->GetPosition());
			sh_model->setUniform("shininess", 32.0f);
			sh_model->stopUsing();

			m_roca2->SetScale(vec3(0.6f));
			m_roca->SetRotation(vec3(0.0f, 90.0f, 0.0f));
			m_roca2->SetPosition(vec3(160.0f, -25.0f, 170.0f));
			m_roca2->Draw(sh_model, camera->GetView(), projection);

			m_roca2->SetScale(vec3(1.0f));
			m_roca2->SetRotation(vec3(0.0f, 90.0f, 0.0f));
			m_roca2->SetPosition(vec3(0.0f, -25.0f, 210.0f));
			m_roca2->Draw(sh_model, camera->GetView(), projection);

			m_roca2->SetScale(vec3(0.8f));
			m_roca2->SetPosition(vec3(-160.0f, -15.0f, 170.0f));
			m_roca2->Draw(sh_model, camera->GetView(), projection);
		}
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
		position.y += (bill_Montania->size.y / 2.0f) - 10.0f;

		position = vec3(0.1f, position.y, -200.0f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);

		position = vec3(-200.0f, position.y, 0.1f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);

		position = vec3(200.0f, position.y, 0.1f);
		bill_Montania->SetPosition(position);
		bill_Montania->Draw(camera, projection, skyColor, lightPosition, true);

		if (CARGA_DIBUJA_SOL_BILL)
		{
			bill_sol->SetPosition(lightPosition);
			bill_sol->Draw(camera, projection, skyColor, lightPosition);
		}
	}

private:

	void Input()
	{
		static HWND* gameWindow = GetGameWindow();

		if (joystick->BotonPresionado(GamePadRR::Boton::BACK))
		{
			DestroyWindow(*gameWindow);
		}
		
		InputColisiones();
		InputMoveDirectionCamara();
		InputGiroJoystick();
		InputLightPosition(true);
		InputLightColor(true);

		// printf("Camara: X: %f, Y: %f, Z: %f\n", camara->GetPosition().x, camara->GetPosition().y, camara->GetPosition().z);
	}

	void InputColisiones()
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
		//	"Casa"
		// --------------------------------------------------
		if (CARGA_DIBUJA_CASA)
		{
			camara->VerificarColision(m_casa->GetPosition(), vec3(25.0, 15.0, 30.0));
		}

		// --------------------------------------------------
		//	"Arboles"
		// --------------------------------------------------
		if (CARGA_DIBUJA_ARBOL)
		{
			for (vec3 posicionArbol : posRndmArboles)
			{
				camara->VerificarColision(posicionArbol, vec3(1.0, 15.0, 1.0), GetDeltaTime());
			}
		}

		// --------------------------------------------------
		//	"Rocas"
		// --------------------------------------------------
		if (CARGA_DIBUJA_ROCAS)
		{
			camara->VerificarColision(vec3(72.0f, 3.0f, -18.0f), vec3(20.0, 10.0, 20.0), GetDeltaTime());
			camara->VerificarColision(vec3(72.0f, 3.0f, -89.0f), vec3(20.0, 10.0, 20.0), GetDeltaTime());
		}

		InputMoveJoystick();
		InputMoveCamara();
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

	void InputMoveCamara()
	{
		if (Input::KeyIsPressed(Input::CustomKey::L_SHIFT))
		{
			camara->SetIsRun(true);
		}
		else
		{
			camara->SetIsRun(false);
		}

		if (Input::KeyIsPressed(Input::CustomKey::W))
		{
			camara->Move(1.0f, 0.0f, GetDeltaTime());
		}

		if (Input::KeyIsPressed(Input::CustomKey::D))
		{
			camara->Move(0.0f, 1.0f, GetDeltaTime());
		}

		if (Input::KeyIsPressed(Input::CustomKey::S))
		{
			camara->Move(-1.0f, 0.0f, GetDeltaTime());
		}

		if (Input::KeyIsPressed(Input::CustomKey::A))
		{
			camara->Move(0.0f, -1.0f, GetDeltaTime());
		}

		if (camara->GetIsFly() && Input::KeyIsPressed(Input::CustomKey::SPACE))
		{
			camara->Float(GetDeltaTime());
		}
	}

	void InputMoveDirectionCamara()
	{
		// direccion con mouse
		double mouseX, mouseY;

		mouseX = Input::GetMouseX();
		mouseY = Input::GetMouseY();

		camara->Rotate((float)-mouseY, (float)mouseX);
	}

	void InputLightPosition(bool show = false)
	{
		if (Input::KeyIsPressed(Input::CustomKey::I))
		{
			lightPosition += vec3(0.0f, 0.0f, 1.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::L))
		{
			lightPosition += vec3(1.0f, 0.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::K))
		{
			lightPosition += vec3(0.0f, 0.0f, -1.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::J))
		{
			lightPosition += vec3(-1.0f, 0.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::U))
		{
			lightPosition += vec3(0.0f, 1.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::O))
		{
			lightPosition += vec3(0.0f, -1.0f, 0.0f);

			if (show)
				printf_s("PosLight(%f, %f, %f)\n", lightPosition.x, lightPosition.y, lightPosition.z);
		}
	}

	void InputLightColor(bool show = false)
	{
		if (Input::KeyIsPressed(Input::CustomKey::UP))
		{
			skyColor = skyColor + vec3(0.01, 0.01, 0.01);

			if (show)
				printf_s("SkyColor(%f, %f, %f)\n", skyColor.x, skyColor.y, skyColor.z);
		}

		if (Input::KeyIsPressed(Input::CustomKey::DOWN))
		{
			skyColor = skyColor + vec3(-0.01, -0.01, -0.01);

			if (show)
				printf_s("SkyColor(%f, %f, %f)\n", skyColor.x, skyColor.y, skyColor.z);
		}
	}
};

#endif // !_FIRSTSCENE_