#version 400

// uniforms
uniform sampler2D shaderMultiTex;
uniform sampler2D shaderTexCesped;
uniform sampler2D shaderNormalCesped;
uniform sampler2D shaderTexRoca;
uniform sampler2D shaderNormalRoca;

uniform vec3 skyColor;

// inputs
in vec3 posicion;
in vec2 texCoord;

in float fogVisibilty;

in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

// outputs
out vec4 outputColor;

// constantes
const float tileTexture = 100.0f;

void main(void)
{
	vec4 texMulti;
	vec4 texCespedColor;
	vec4 texCespedNormal;
	vec4 texRocaColor;
	vec4 texRocaNormal;
	vec2 tiledCoords;
	float backTexAmount;

	// cargamos color de multitextura
	texMulti = texture(shaderMultiTex, texCoord);
	tiledCoords = texCoord * tileTexture;

	// obtenemos la cantidad de negro del color posterior (es decir la textura primaria)
	backTexAmount = 1 - (texMulti.r + texMulti.g + texMulti.b); // solo ocupamos el azul pero de una vez todos

	// cargamos textura y normal del cesped
	texCespedColor = texture(shaderTexCesped, tiledCoords) * backTexAmount;
	texCespedNormal = texture(shaderNormalCesped, tiledCoords);
	
	// cargamos textura y normal de la roca
	texRocaColor = texture(shaderTexRoca, tiledCoords);
	texRocaNormal = texture(shaderNormalRoca, tiledCoords);

	// declaramos vectores para color y normales finales
	vec3 color;
	vec3 normal;

	// obtenemos los colores en base al color del mapa de multi textura
	vec4 bTexColor = texRocaColor * texMulti.b;
	// vec4 bTexColor = vec4(0.0, 0.0f, 1.0, 1.0f) * texMulti.b; // prueba
	vec4 bnTexColor = texRocaNormal * texMulti.b;
	
	color = texCespedColor.rgb + bTexColor.rgb;
	normal = texCespedNormal.rgb + bnTexColor.rgb;
	normal = normalize(normal * 2.0f - 1.0f);

	// calculamos luz ambiental
	vec3 ambient = 0.1f * color * skyColor;

	// calculamos direccion de la luz
	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);

	// calculamos el factor de luz difusa
	float diff = max(dot(lightDir, normal), 0.0f); 

	// obtenemos la luz difusa final
	vec3 diffuse = diff * color * skyColor;

	// aplicamos ambiente, difusa y specular
	outputColor = vec4(ambient + diffuse, 1.0f);

	// aplicamos fog
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}