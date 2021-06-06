#zwrite on
#cull off
#setProjectionMatrix on
#shader vertex
#version 430 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 norm;

out vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
    normal = transpose(inverse(mat3(modelMatrix))) * norm;
    gl_Position = projectionMatrix * modelMatrix * pos;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

in vec3 normal;

uniform vec3 diffuse;

void main()
{
    // diffuse 
    vec3 lightDir = normalize(vec3(-0.5, -1, -0.5));
    float diff = 0.6 + 0.1 * max(dot(normal, lightDir), 0.0);

    colour = vec4(diffuse * diff, 1.0);
}