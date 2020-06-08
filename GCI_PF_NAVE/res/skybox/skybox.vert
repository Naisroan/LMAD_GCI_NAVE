#version 400

in vec3 aPos;

out vec3 texCoords;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	vec4 pos;

	texCoords = aPos;
	pos = transform * projection * view * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}