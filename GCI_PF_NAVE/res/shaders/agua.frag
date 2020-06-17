#version 400

// uniforms
uniform sampler2D shaderTexture;
uniform sampler2D shaderNormal;
uniform sampler2D shaderSpecular;

uniform vec3 skyColor;

uniform vec3 lightPos;		// Posicion de la luz
uniform vec3 viewPos;		// Posicion de la camara

// inputs
in vec3 posicion;
in vec2 texCoord;
in vec3 normal;
in vec3 tangente;
in vec3 binormal;
in float fogVisibilty;

// outputs
out vec4 outputColor;

void main(void)
{
	// obtenemos color de texturas
	vec4 texColor = texture(shaderTexture, texCoord);
	vec4 texNormal = texture(shaderNormal, texCoord);
	vec4 texSpecular = 1 - texture(shaderSpecular, texCoord);

	// obtenemos aportacion ambiental
	vec4 ambientAport = 0.2f * texColor * vec4(skyColor, 1.0f);

	// calculamos la direccion de la luz en base a la TBN calculada en el vert
	// vec3 lightDirection = normalize(lightPos - posicion);
	vec3 lightDirection = normalize(-lightPos);

	// obtenemos aportacion difusa
	float diff = clamp(dot(lightDirection, normal), 0.0f, 1.0f);

	// obtenemos la luz difusa final
	vec4 diffuseAport = diff * texColor * vec4(skyColor, 1.0f);
	
	// specular
	float shininess = 40.0f;
	float FAS = 0.6f;

	vec3 viewDirection = normalize(viewPos - posicion);
	vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));
	float spec = pow(max(dot(-viewDirection, reflectionDirection), 0.0f), shininess);
	
	// obtenemos la luz specular final
	vec4 specularAport = texSpecular * vec4(skyColor, 1.0f) * FAS * spec;

	// calculamos color final y le aplicamos fog
	outputColor = (ambientAport + diffuseAport + specularAport);
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}