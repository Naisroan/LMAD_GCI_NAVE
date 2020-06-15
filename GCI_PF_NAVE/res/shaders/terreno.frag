#version 400

// uniforms
uniform sampler2D shaderMultiTex;

uniform sampler2D shaderTexCesped;
uniform sampler2D shaderNormalCesped;

uniform sampler2D shaderTexRoca;
uniform sampler2D shaderNormalRoca;

uniform vec3 skyColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// inputs
in vec3 posicion;
in vec2 texCoord;
in vec3 normal;
in vec3 tangente;
in vec3 binormal;
in float fogVisibilty;

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

	// cargamos textura, normal y specular del cesped
	texCespedColor = texture(shaderTexCesped, tiledCoords) * backTexAmount;
	texCespedNormal = texture(shaderNormalCesped, tiledCoords);
	
	// cargamos textura, normal y specular de la roca
	texRocaColor = texture(shaderTexRoca, tiledCoords);
	texRocaNormal = texture(shaderNormalRoca, tiledCoords);

	// declaramos vectores para color y normales finales
	vec3 color;
	vec3 colorNormal;

	// obtenemos los colores en base al color del mapa de multi textura
	vec4 bTexColor = texRocaColor * texMulti.b; // combinacion de textura
	vec4 bnTexColor = texRocaNormal * texMulti.b; // combinacion  de normal
	
	// lo que hacemos aqui es combinar la normal y specular del cesped con el de la roca dependiendo del azul
	colorNormal = texCespedNormal.rgb + bnTexColor.rgb;

	// aplicamos este calculo para que se note la normal
	colorNormal = colorNormal * 2.0f - 1.0f;
	colorNormal.x = -1.0f * colorNormal.x;
	// colorNormal.x *= -1.0f * colorNormal.x;
	
	// obtenemos el color final combinando las dos texturas dependiendo del azul
	color = texCespedColor.rgb + bTexColor.rgb;

	// calculamos luz ambiental
	vec3 ambient = 0.2f * color * skyColor;

	// matriz TBN
	mat3 TBN;
	TBN = mat3(tangente, binormal, normal);

	// aplicamos la matriz TBN a la normal
	colorNormal = normalize(TBN * colorNormal);

	// calculamos direccion de la luz
	// vec3 lightDir = normalize(-lightPos - posicion);
	vec3 lightDir = normalize(-lightPos);

	// calculamos el factor de luz difusa
	float diff = clamp(dot(lightDir, colorNormal), 0.0f, 1.0f); 

	// obtenemos la luz difusa final
	vec3 diffuse = diff * color * skyColor;

	// aplicamos ambiente, difusa y specular
	outputColor = vec4(ambient + diffuse, 1.0f);

	// aplicamos fog
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}