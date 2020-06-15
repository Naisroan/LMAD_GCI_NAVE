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
	outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * 10.0f;
}