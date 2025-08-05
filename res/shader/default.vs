#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 C;
uniform vec4 T;

layout(location=1) in vec4 position;
layout(location=2) in vec2 texcoord;
layout(location=3) in vec4 color;
layout(location=4) in vec3 normal;

out vec4 vColor;
out vec2 vTexcoord;

void main () {
    vColor = color * C;
    vTexcoord = texcoord;

    gl_Position = P*V*M*position;
}
