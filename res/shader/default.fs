#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 C;
uniform vec4 T;

uniform sampler2D image;
uniform bool use_image;

in vec4 vColor;
in vec2 vTexcoord;

out vec4 fColor;

void main () {
    fColor = vColor;
}
