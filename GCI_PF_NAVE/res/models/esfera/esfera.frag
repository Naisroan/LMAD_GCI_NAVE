#version 400

// uniforms
uniform sampler2D shaderTexture;
uniform sampler2D shaderNormal;
uniform sampler2D shaderSpecular;

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

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	vec4 texColor;
	vec4 texNormal;

	// cargamos color de textura para el vertice (textura del cesped, y la textura de la normal)
	texColor = texture(shaderTexture, texCoord);
	texNormal = texture(shaderNormal, texCoord);

	// calculamos la normal
	vec3 normal;
	normal = texNormal.rgb;
	normal = normalize(normal * 2.0f - 1.0f);

	// obtenemos el color correspondiente a la textura de acuerdo al vertice actual
	vec3 color = texColor.rgb;

	// calculamos luz ambiental
	vec3 ambient = 0.1f * color;

	// calculamos direccion de la luz
	vec3 lightDir = normalize(tangentLightPos - tangentFragPos);

	// calculamos el factor de luz difusa
	float diff = clamp(dot(lightDir, normal), 0.0f, 1.0f);

	// obtenemos la luz difusa final
	vec3 diffuse = diff * color;

	// aplicamos ambiente, difusa y specular
	outputColor = vec4(ambient + diffuse, 1.0f);

	// aplicamos fog
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}