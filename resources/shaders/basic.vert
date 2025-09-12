#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

out vec3 oFragPosition;
out vec3 oNormal;
out vec2 oTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    oFragPosition = vec3(uView * uModel * vec4(aPos, 1.0));     // convert to view space coordinates
    oNormal = normalize(aNormal);                               // normal attribute is not normalized
    oTexCoord = aTexCoord;
}