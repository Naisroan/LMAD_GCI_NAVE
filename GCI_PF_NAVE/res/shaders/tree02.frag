#version 440

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambientCoe;

uniform vec3 cameraPosition;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

uniform float shininess;
uniform float transparency;

uniform bool hasNormalMap;
uniform bool hasAmbientMap;
uniform bool hasDiffuseMap;
uniform bool hasSpecularMap;

uniform sampler2D normalMap;
uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragTangent;
in vec3 fragBinormal;

in float fogVisibilty;

out vec4 finalColor;

void main() 
{
    vec4 diffuseAlpha = texture(diffuseMap, fragTexCoord);

    if (diffuseAlpha.a <= 0.1f)
    {
        discard;
    }

    float diffuseCoe, specularCoe, specularPixel, shininessPixel, transparencyPixel;
    vec3 ambientResult, diffuseResult, specularResult, normal;
    vec3 lightDirection, lightReflection, viewDirection;
    vec3 normalPixel, ambientPixel, diffusePixel;
    vec3 lighColorFinal = lightColor * 0.01;
    
    normalPixel = hasNormalMap ? texture(normalMap, fragTexCoord).xyz : vec3(0, 0, 0.5);
    diffusePixel = hasDiffuseMap ? texture(diffuseMap, fragTexCoord).xyz : vec3(1.0f, 1.0f, 1.0f);
    ambientPixel = hasAmbientMap ? texture(ambientMap, fragTexCoord).xyz : vec3(1.0f, 1.0f, 1.0f) * diffusePixel;
    specularPixel = hasSpecularMap ? texture(specularMap, fragTexCoord).x : 1.0f;
    
    normal = fragNormal;

    if (hasNormalMap) 
    {
        normalPixel = (normalPixel * 2.0) - 1.0;

        normal = normalize(
        normalPixel.x * fragTangent +
        normalPixel.y * fragBinormal +
        normalPixel.z * fragNormal);
    } 

    ambientResult = ambientCoe * ambient * ambientPixel * lighColorFinal;

	// ligero fix
	vec3 lightPosFix = lightPosition;
	lightPosFix.y = lightPosition.y * -1;
	lightPosFix.z = lightPosition.z * -1;
    
    lightDirection = -lightPosFix;

    diffuseCoe = max(0.0, dot(normal, lightDirection));
    diffuseResult = diffuseCoe * diffuse * diffusePixel * lighColorFinal;
    
    specularResult = vec3(0.0, 0.0, 0.0);
    
    if(diffuseCoe > 0.0) 
    {
    	lightReflection = normalize(reflect(-lightDirection, normal));
        viewDirection = normalize(cameraPosition - fragPosition);   
    	specularCoe = max(0.0, dot(viewDirection, lightReflection));
    	specularCoe = pow(specularCoe, shininess * shininessPixel);
    	specularResult = specularCoe * specular * specularPixel * lighColorFinal;
    }

    vec3 phong = clamp(ambientResult + diffuseResult + specularResult, 0.0, 1.0);

    finalColor = vec4(phong, transparency * transparencyPixel);
    // finalColor = vec4(pow(phong, vec3(1.0/2.2)), transparency * transparencyPixel);

	// aplicamos fog
	finalColor = mix(vec4(lightColor, 1.0f), finalColor, fogVisibilty);
}