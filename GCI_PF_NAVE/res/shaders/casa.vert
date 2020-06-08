#version 440

// uniforms
uniform mat4 camera;
uniform mat4 model;
uniform mat4 projection;

// ins
in vec3 vertPosition;
in vec2 vertTexCoord;
in vec3 vertNormal;
in vec3 vertTangent;
in vec3 vertBinormal;

// outs
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragTangent;
out vec3 fragBinormal;
out float fogVisibilty;

// constantes
const float fogDensity = 0.005f;
const float fogGradient = 1.5f;

void main() 
{
	vec4 worldPosition;	
	vec4 posRelativeToCam;

	worldPosition = model * vec4(vertPosition, 1.0f);
	posRelativeToCam = camera * worldPosition;

	// calculamos la distancia para el fog en base a la densidad y gradiente
	float _distance = length(posRelativeToCam.xyz);
	fogVisibilty = exp(-pow(_distance * fogDensity, fogGradient));
	fogVisibilty = clamp(fogVisibilty, 0.0f, 1.0f);

	mat3 normalMatrix = transpose(inverse(mat3(model)));

	fragPosition = normalMatrix * vec3(worldPosition);
	fragTexCoord = vertTexCoord;
	fragNormal = normalMatrix * vertNormal;
	fragTangent = normalMatrix * vertTangent;
	fragBinormal = normalMatrix * vertBinormal;
	
	gl_Position = projection * posRelativeToCam;
}