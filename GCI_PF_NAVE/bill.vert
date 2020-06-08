#version 440

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

in vec3 vertPosition;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

out float fogVisibilty;

// constantes
const float fogDensity = 0.005f;
const float fogGradient = 1.5f;

void main() 
{
    fragTexCoord = vertTexCoord;

	vec4 worldPosition;	
	vec4 posRelativeToCam;

	worldPosition = model * vec4(vertPosition, 1.0f);
	posRelativeToCam = camera * worldPosition;

	// calculamos la distancia para el fog en base a la densidad y gradiente
	float _distance = length(posRelativeToCam.xyz);
	fogVisibilty = exp(-pow(_distance * fogDensity, fogGradient));
	fogVisibilty = clamp(fogVisibilty, 0.0f, 1.0f);
	
	gl_Position = projection * posRelativeToCam;
}