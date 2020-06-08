#version 400

// inputs
in vec3 inputPosition;
in vec3 inputNormal;
in vec2 inputTexCoord;
in vec3 inputTangent;
in vec3 inputBinormal;

// outputs
out vec2 texCoord;
out vec3 normal;

out float fogVisibilty;

out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

// uniforms
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPos;		// Posicion de la luz
uniform vec3 viewPos;		// Posicion de la camara

uniform float movementVertex;
uniform float movementTexture;

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

	// establecemos valores de outs para el fragment shader
	texCoord = inputTexCoord;
	texCoord.y += movementTexture;

	// calculamos la distancia para el fog en base a la densidad y gradiente
	float _distance = length(posRelativeToCam.xyz);
	fogVisibilty = exp(-pow(_distance * fogDensity, fogGradient));
	fogVisibilty = clamp(fogVisibilty, 0.0f, 1.0f);

	// mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
	mat3 normalMatrix = mat3(worldMatrix);

	// calculamos TBN (para el mapa de normales)
	vec3 T = normalize(normalMatrix * inputTangent);
	vec3 N = normalize(normalMatrix * inputNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	// ligero fix
	vec3 lightPosFix = lightPos;
	lightPosFix.y = lightPos.y * -1;
	lightPosFix.z = lightPos.z * -1;

	mat3 TBN = mat3(T, B, N);

	tangentLightPos = TBN * lightPosFix;
    tangentViewPos  = TBN * viewPos;
    tangentFragPos  = TBN * vec3(worldPosition);
}