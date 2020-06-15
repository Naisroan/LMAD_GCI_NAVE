#version 400

// uniforms
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float movementVertex;
uniform float movementTexture;

// inputs
in vec3 inputPosition;
in vec3 inputNormal;
in vec2 inputTexCoord;
in vec3 inputTangent;
in vec3 inputBinormal;

// outputs
out vec3 posicion;
out vec2 texCoord;
out vec3 normal;
out vec3 tangente;
out vec3 binormal;

out float fogVisibilty;

// constantes
const float fogDensity = 0.005f;
const float fogGradient = 1.5f;

void main(void)
{
	vec4 worldPosition;		// posicion del vertice respecto al mundo
	vec4 posRelativeToCam;	// posicion del vertice respecto a la camara

	worldPosition = worldMatrix * vec4(inputPosition, 1.0f);
	posRelativeToCam = viewMatrix * worldPosition;

	// establecemos posicion en base a la proyeccion (pantalla) y camara
	gl_Position = projectionMatrix * posRelativeToCam;
	gl_Position.y += movementVertex;
	gl_Position.x += movementVertex * -1.0f;

	// calculamos la distancia para el fog en base a la densidad y gradiente
	float _distance = length(posRelativeToCam.xyz);
	fogVisibilty = exp(-pow(_distance * fogDensity, fogGradient));
	fogVisibilty = clamp(fogVisibilty, 0.0f, 1.0f);

	// establecemos valores de outs para el fragment shader
	posicion = vec3(worldPosition);
	texCoord = inputTexCoord;
	texCoord.y += movementTexture;
	normal = normalize(vec3(worldMatrix * vec4(inputNormal, 0.0f)));
	tangente = normalize(vec3(worldMatrix * vec4(inputTangent, 0)));
	tangente = normalize(tangente - normal * dot(normal,tangente));
	binormal = normalize(cross(normal, tangente));
}