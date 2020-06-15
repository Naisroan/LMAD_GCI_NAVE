#version 440

// uniforms
uniform sampler2D normalMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D ambientMap;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

uniform bool tieneNormalMap;
uniform bool tieneAmbientMap;
uniform bool tieneDiffuseMap;
uniform bool tieneSpecularMap;

uniform vec3 lightPos;
uniform vec3 skyColor;
uniform vec3 viewPos;

// inputs
in vec3 posicion;
in vec2 texCoord;
in vec3 normal;
in vec3 tangente;
in vec3 binormal;
in float fogVisibilty;

out vec4 outputColor;

void main() 
{
	// obtenemos color de texturas
	vec4 texColor = tieneDiffuseMap ? texture(diffuseMap, texCoord) : vec4(diffuse, 1.0);

	if (texColor.a <= 0.1f)
    {
        discard;
    }

	vec4 texNormal = tieneNormalMap ? texture(normalMap, texCoord) : vec4(0.5f, 0.5f, 1.0f, 1.0f);
	vec4 texSpecular = tieneSpecularMap ? 1 - texture(specularMap, texCoord) : vec4(0.0f);

	// obtenemos aportacion ambiental
	vec4 ambientAport = 0.2f * texColor * vec4(skyColor, 1.0f);

	// obtenemos color de la normal
	vec3 colorNormal = texNormal.rgb;

	// aplicamos este calculo para que se note la normal
	colorNormal = colorNormal * 2.0f - 1.0f;
	colorNormal.x = -1.5f * colorNormal.x;

	// matriz TBN
	mat3 TBN;
	TBN = mat3(tangente, binormal, normal);
	
	// aplicamos la matriz TBN a la normal
	colorNormal = normalize(TBN * colorNormal);

	// calculamos la direccion de la luz en base a la TBN calculada en el vert
	// vec3 lightDirection = normalize(lightPos - posicion);
	vec3 lightDirection = normalize(lightPos);

	// obtenemos aportacion difusa
	float diff = clamp(dot(lightDirection, colorNormal), 0.0f, 1.0f);

	// obtenemos la luz difusa final
	vec4 diffuseAport = diff * texColor * vec4(skyColor, 1.0f);
	
	// specular
	float shininess = 100.0f;
	float FAS = 1.0f;

	vec3 viewDirection = normalize(viewPos - posicion);
	vec3 reflectionDirection = normalize(reflect(lightDirection, colorNormal));
	float spec = pow(max(dot(-viewDirection, reflectionDirection), 0.0f), shininess);
	
	// obtenemos la luz specular final
	vec4 specularAport = texSpecular * vec4(skyColor, 1.0f) * FAS * spec;

	// calculamos color final y le aplicamos fog
	outputColor = (ambientAport + diffuseAport + specularAport);
	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}