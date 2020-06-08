#version 400

// uniforms
uniform sampler2D shaderTexture;

uniform vec3 skyColor;

// inputs
in vec3 posicion;
in vec2 texCoord;
in vec3 normal;
in float fogVisibilty;

// outputs
out vec4 outputColor;

void main(void)
{
	vec4 textureColor;

	textureColor = texture(shaderTexture, texCoord);
	outputColor = textureColor * vec4(skyColor, 1.0f);

//	outputColor = textureColor;
//	outputColor = mix(vec4(skyColor, 1.0f), outputColor, fogVisibilty);
}