#zwrite on
#cull off
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

uniform mat4 projectionMatrix;
varying vec4 nColour;

void main() {
    nColour = normal * 0.5 + 0.5;
    gl_Position = projectionMatrix * position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

uniform vec4 setColour;
varying vec4 nColour;

void main() {
    colour = nColour;
}