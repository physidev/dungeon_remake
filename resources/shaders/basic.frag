#version 330 core
in vec3 oFragPosition;
in vec3 oNormal;        // guaranteed to be unit vector
in vec2 oTexCoord;

out vec4 FragColor;

struct Lamp {
    vec3 position;
    vec3 color;

    vec3 attenuation;
};

uniform sampler2D uTexture;
uniform Lamp uLamp;

void main() {
    // POINT-SOURCE PHONG LIGHTING
    // attenuation
    float distance = length(uLamp.position - oFragPosition);
    float attenuation = 1.0/(
        uLamp.attenuation.x +
        uLamp.attenuation.y * distance +
        uLamp.attenuation.z * distance * distance
    );

    // ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLamp.color;

    // diffuse lighting
    vec3 lightDirection = normalize(uLamp.position - oFragPosition);
    float diffuseStrength = max(dot(oNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * uLamp.color;

    vec3 brightness = ambient + attenuation * diffuse;

    FragColor = vec4(brightness, 1.0) * texture(uTexture, oTexCoord);
}