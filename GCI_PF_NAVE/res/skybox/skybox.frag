#version 400

out vec4 outputColor;

in vec3 texCoords;

uniform samplerCube sky;    // dia
uniform samplerCube sky2;   // tarde
uniform samplerCube sky3;   // noche
uniform float skyTime;
uniform bool toNight;
uniform vec3 fogColor;

const float lowerLimit = 0.0f;
const float upperLimit = 0.5f;

void main(void)
{
    vec4 colorDay = texture(sky, texCoords);
    vec4 colorAfternoon = texture(sky2, texCoords);
    vec4 colorNight = texture(sky3, texCoords);

    vec4 combinacion;

    if (toNight) // hacia noche
    {
        if (skyTime > 1.0f) // si es mayor o igual a 2 quiere que decir que apenas es de dia y va hacia tarde
        {
            combinacion = mix(colorAfternoon, colorDay, skyTime - 1.0f);
        }

        if (skyTime <= 1.0f) // si es menor o igual a 1 quiere que decir que ya es de tarde y va hacia noche
        {
            combinacion = mix(colorNight, colorAfternoon, skyTime);
        }
    }
    else  // hacia dia
    {
        if (skyTime < 1.0f) // si es menor que 1 quiere decir que es de noche y va hacia tarde (o mañana/madrugada)
        {
            combinacion = mix(colorNight, colorAfternoon, skyTime);
        }

        if (skyTime >= 1.0f) // si es mayor o igual a 1 quiere decir que es de mañana/madrugada y va hacia dia
        {
            combinacion = mix(colorAfternoon, colorDay, skyTime - 1.0f);
        }
    }

    float factor = (texCoords.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0f, 1.0f);

    outputColor = mix(vec4(fogColor, 1.0f), combinacion, factor);
}