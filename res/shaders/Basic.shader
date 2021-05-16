#zwrite on
#cull on
#setProjectionMatrix on
#shader vertex
#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 norm;

#define lightNum 4

out VS_OUT{
    vec3 position;
    vec3 normal;
    vec4 positionLightSpace[lightNum];
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix[lightNum];

uniform int usedLights;

void main()
{
    vs_out.position = vec3(modelMatrix * pos);
    vs_out.normal = transpose(inverse(mat3(modelMatrix))) * norm;
    for (int i = 0; i < lightNum; i++)
    {
        if (i == usedLights)
            break;
        vs_out.positionLightSpace[i] = lightSpaceMatrix[i] * vec4(vs_out.position, 1.0);
    }
    gl_Position = projectionMatrix * vec4(vs_out.position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

#define lightNum 4

in VS_OUT{
    vec3 position;
    vec3 normal;
    vec4 positionLightSpace[lightNum];
} fs_in;

uniform sampler2D shadowMap[lightNum];

uniform vec3 lightDir[lightNum];
uniform vec3 viewPos;

uniform int usedLights;

vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216),
    vec2(0.94558609, -0.76890725),
    vec2(-0.094184101, -0.92938870),
    vec2(0.34495938, 0.29387760),
    vec2(-0.91588581, 0.45771432),
    vec2(-0.81544232, -0.87912464),
    vec2(-0.38277543, 0.27676845),
    vec2(0.97484398, 0.75648379),
    vec2(0.44323325, -0.97511554),
    vec2(0.53742981, -0.47373420),
    vec2(-0.26496911, -0.41893023),
    vec2(0.79197514, 0.19090188),
    vec2(-0.24188840, 0.99706507),
    vec2(-0.81409955, 0.91437590),
    vec2(0.19984126, 0.78641367),
    vec2(0.14383161, -0.14100790)
);

float ShadowCalculation(vec4 fragPosLightSpace, uint index)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // check whether current frag pos is in shadow
    float bias = max(0.002 * (1.0 - dot(fs_in.normal, lightDir[index])), 0.0005);
    float shadow = 0;

    for (int i = 0; i < 16; i++) {
        float val = texture(shadowMap[index], projCoords.xy + poissonDisk[i] / 4000.0).r;
        if (val < projCoords.z - bias) {
            shadow += 0.05;
        }
    }

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    vec3 color = vec3(1.0);
    vec3 normal = fs_in.normal;
    vec3 lightColor = vec3(1.0);

    // ambient
    vec3 ambient = 0.06 * color;
    vec3 lighting = ambient;
    for (int i = 0; i < lightNum; i++)
    {
        if (i == usedLights)
            break;

        // diffuse
        float diff = max(dot(lightDir[i], normal), 0.0);
        vec3 diffuse = diff * lightColor;

        // calculate shadow
        float shadow = ShadowCalculation(fs_in.positionLightSpace[i], i);
        lighting += ((1.0 - shadow) * diffuse) * color;
    }

    colour = vec4(lighting / usedLights, 1.0);
}