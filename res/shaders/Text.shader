#zwrite off
#cull off
#blend on
#shader vertex
#version 330 core

layout(location = 0) in vec4 vert;
out vec2 UV;

void main() {
	gl_Position = vec4(vert.xy, 0, 1);
	UV = vert.zw;
}

#shader fragment
#version 330 core

in vec2 UV;
out vec4 color;

uniform sampler2D char;
uniform vec3 textColour;

void main() {
	color = vec4(textColour, 1.0) * vec4(1.0, 1.0, 1.0, texture(char, UV).r);
}