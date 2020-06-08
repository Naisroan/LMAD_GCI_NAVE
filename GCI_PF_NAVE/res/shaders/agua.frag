#version 400

// uniforms
uniform sampler2D shaderTexture;
uniform sampler2D shaderSpecular;

uniform vec3 skyColor;

// inputs
in vec2 texCoord;
in vec3 normal;

in float fogVisibilty;

in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

// outputs
out vec4 outputColor;

void main(void)
{
	// obtenemos color de texturas
	vec4 texColor = texture(shaderTexture, texCoord);
	vec4 texSpecular = texture(shaderSpecular, texCoord);

	// obtenemos aportacion ambiental
	vec4 ambientAport = 0.1f * texColor * vec4(skyColor, 1.0f);

	// calculamos la direccion de la luz en base a la TBN calculada en el vert
	vec3 lightDirection = normalize(tangentLightPos - tangentFragPos);

	// obtenemos aportacion difusa
	float diff = max(dot(lightDirection, normal), 0.0f);
	vec4 diffuseAport = diff * texColor * vec4(skyColor, 1.0f);

	// obtenemos aportacion difusa
	float shininess = 0.6f;

	vec3 viewDirection = normalize(tangentViewPos - tangentFragPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), shininess);
	vec4 specularAport = spec * texSpecular * vec4(skyColor, 1.0f);

	// calculamos color final y le aplicamos fog
	outputColor = (ambientAport + diffuseAport + specularAport);
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}