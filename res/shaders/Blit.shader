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
out vec3 color;

uniform sampler2D renderedTexture;

/*
vec3 blendsample(vec2 uv)
{
	vec3 o = vec3(0);
	o += texture(renderedTexture, uv).xyz;
	o += texture(renderedTexture, uv + vec2(1, 0) / 50).xyz;
	o += texture(renderedTexture, uv + vec2(0, 1) / 50).xyz;
	o += texture(renderedTexture, uv + vec2(-1, 0) / 50).xyz;
	o += texture(renderedTexture, uv + vec2(0, -1) / 50).xyz;
	return o * 0.2;
}*/

void main() {
	color = texture(renderedTexture, UV).xyz;
}