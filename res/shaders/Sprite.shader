#zwrite off
#cull off
#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
out vec2 UV;

void main() {
	gl_Position = vec4(pos, 1);
	UV = (pos.xy + vec2(1, 1)) / 2.0;
}

#shader fragment
#version 330 core

in vec2 UV;
out vec4 color;

//uniform sampler2D renderedTexture;

void main() {
	color = vec4(1, 1, 0, 0.5);
}