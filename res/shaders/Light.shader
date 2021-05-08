#zwrite on
#cull off
#setProjectionMatrix on
#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = projectionMatrix * modelMatrix * position;
}

#shader fragment
#version 330 core

void main() {
}