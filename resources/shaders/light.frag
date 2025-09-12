#version 330 core
out vec4 FragColor;

in vec3 oColor;
in vec2 oTexCoord;

uniform sampler2D uTexture;

void main() {
    FragColor = vec4(1.0) * texture(uTexture, oTexCoord);
}