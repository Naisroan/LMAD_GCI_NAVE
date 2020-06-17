#include "billboard.h"

Billboard::Billboard(Shader* shader, string rutaTextura, vec2 billboardSize)
{
	gQuad = new Quad(shader);
	gProgram = shader;

	size = billboardSize;
	textura = new Texture(Bitmap::bitmapFromFile(rutaTextura), GL_LINEAR);
}

Billboard::~Billboard()
{
	delete gQuad;
	delete textura;
}

void Billboard::Draw(Camera* camera, mat4 projection, vec3 skyColor, vec3 lightPosition, bool toCenter)
{
	vec3 up = vec3(0, 1, 0);
	float angle = 0.0f;

	vec3 cameraPosition = !toCenter ? camera->GetPosition() : vec3(0.0f, 0.0f, 0.0f);
	vec3 billPosition = GetPosition();

	vec3 objToCam = cameraPosition - billPosition;
	objToCam.y = 0;

	objToCam = normalize(objToCam);

	float angleCosine = dot(vec3(0, 0, -1), objToCam);
	up = cross(vec3(0, 0, -1), objToCam);

	angle = acos(angleCosine) * (180.0 / pi<float>());

	mat4 modelMatrix =
		translate(mat4(1.0f), billPosition) *
		rotate(mat4(1.0f), radians(angle), up) *
		translate(mat4(1.0f), vec3(-size.x / 2.0f, -size.y / 2.0f, 0)) *
		scale(mat4(1.0f), vec3(size.x, size.y, 1));
	
	gProgram->use();
	gProgram->setUniform("alpha", 1.0f);
	gProgram->setUniform("lightPosition", lightPosition);
	gProgram->setUniform("lightColor", skyColor);
	gProgram->stopUsing();

	gQuad->Draw(textura->object(), modelMatrix, camera->GetView(), projection);
}