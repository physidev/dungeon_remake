#version 330 core
in vec3 oColor;
in vec2 oTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor;

void main() {
    FragColor = vec4(uLightColor, 1.0) * texture(uTexture, oTexCoord);
}