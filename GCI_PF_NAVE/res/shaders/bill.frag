#version 440

uniform vec3 color;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform sampler2D map;

in vec2 fragTexCoord;

in float fogVisibilty;

out vec4 finalColor;

void main() 
{
    vec4 mapPixel = texture(map, fragTexCoord);
    float transparency = mapPixel.a;
    vec3 pixel = mapPixel.rgb * lightColor;
    
    if (transparency <= 0.1) 
        discard;

    finalColor = vec4(pixel, transparency);
	finalColor = mix(vec4(lightColor, 1.0f), finalColor, fogVisibilty);
}