#version 400

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
out float fogVisibilty;

// uniforms
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float movementTexture;

// constantes
const float fogDensity = 0.00001f;
const float fogGradient = 1.5f;

void main(void)
{
	vec4 worldPosition;
	vec4 posRelativeToCam;

	worldPosition = worldMatrix * vec4(inputPosition, 1.0f);
	posRelativeToCam = viewMatrix * worldPosition;

	gl_Position = projectionMatrix * posRelativeToCam;

	// establecemos valores de outs para el fragment shader
	posicion = vec3(gl_Position);
	normal = vec3(worldMatrix * vec4(inputNormal, 0));
	texCoord = inputTexCoord;
	texCoord.y += movementTexture;

//	float _distance = length(posRelativeToCam.xyz);
//	fogVisibilty = exp(-pow(_distance * fogDensity, fogGradient));
//	fogVisibilty = clamp(fogVisibilty, 0.0f, 1.0f);
}