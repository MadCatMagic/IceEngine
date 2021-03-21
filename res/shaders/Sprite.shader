#zwrite off
#cull off
#blend on
#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
out vec2 UV;

void main() {
	gl_Position = vec4(pos, 1);
	UV = uv;
}

#shader fragment
#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D sprite;

void main() {
	color = texture(sprite, UV);
}