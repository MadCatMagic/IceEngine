#zwrite on
#cull on
#setProjectionMatrix on
#shader vertex
#version 430 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

#define lightNum 2

out VS_OUT{
    vec3 position;
    vec2 uv;
    vec3 normal;
    vec4 positionLightSpace[lightNum];
    vec3 tangentLightPos[lightNum];
    vec3 tangentLightDir[lightNum];
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix[lightNum];

uniform int usedLights;

uniform vec3 posL[lightNum];
uniform vec3 dirL[lightNum];
uniform vec3 viewPos;

void main()
{
    vs_out.position = vec3(modelMatrix * pos);
    vs_out.normal = transpose(inverse(mat3(modelMatrix))) * norm;
    vs_out.uv = uv;
    gl_Position = projectionMatrix * vec4(vs_out.position, 1.0);

    vec3 T = normalize((modelMatrix * vec4(tangent, 0.0)).xyz);
    vec3 B = normalize((modelMatrix * vec4(normalize(cross(norm, tangent)), 0.0)).xyz);
    vec3 N = normalize((modelMatrix * vec4(norm, 0.0)).xyz);
    mat3 TBN = transpose(mat3(T, B, N));

    for (int i = 0; i < lightNum; i++)
    {
        if (i == usedLights)
            break;
        vs_out.positionLightSpace[i] = lightSpaceMatrix[i] * vec4(vs_out.position, 1.0);
        vs_out.tangentLightPos[i] = TBN * posL[i];
        vs_out.tangentLightDir[i] = TBN * dirL[i];
    }

    vs_out.tangentViewPos = TBN * viewPos;
    vs_out.tangentFragPos = TBN * (modelMatrix * pos).xyz;
}

#shader fragment
#version 430 core

layout(location = 0) out vec4 colour;

#define lightNum 2

in VS_OUT{
    vec3 position;
    vec2 uv;
    vec3 normal;
    vec4 positionLightSpace[lightNum];
    vec3 tangentLightPos[lightNum];
    vec3 tangentLightDir[lightNum];
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;
uniform sampler2D textureNormal;

uniform sampler2D shadowMap[lightNum];

uniform int usedLights;

// kind of array struct 
// opengl doesnt like arrays of structs so
uniform vec3 posL[lightNum];
uniform vec3 dirL[lightNum];
uniform float cutoffL[lightNum];
uniform float outerCutoffL[lightNum];

// alpha channel is strength
uniform vec4 colourL[lightNum];

// 0 = sun, 1 = spot, 2 = point
uniform int typeL[lightNum];

uniform float constantAtten;
uniform float linearAtten;
uniform float quadraticAtten;

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

//float random(vec3 seed, int i) {
//    vec4 seed4 = vec4(seed, i);
//    float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
//    return fract(sin(dot_product) * 43758.5453);
//}

float ShadowCalculation(vec4 fragPosLightSpace, uint index, vec3 normal)
{
    // transform pos into better SPACE
    float bias = max(0.002 * (1.0 - dot(normal, fs_in.tangentLightDir[index])), 0.0005);
    vec3 projCoords = (fragPosLightSpace.xyz - vec3(0, 0, bias)) / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // check whether current frag pos is in shadow
    float shadow = 0;

    for (int i = 0; i < 16; i++) {
        //int r = int(16.0 * random(projCoords.xyz * 10, i)) % 16;
        float val = texture(shadowMap[index], projCoords.xy + poissonDisk[i] / 10000.0).r;
        if (val < max(projCoords.z, 0.1)) {
            shadow += 0.0625;
        }
    }

    //if (projCoords.z = 0.0)
    //    shadow = 1.0;

    return shadow;
}

void main()
{
    // triplanar mappings
    vec3 colX = texture(textureDiffuse, fs_in.position.zy).rgb;
    vec3 colY = texture(textureDiffuse, fs_in.position.xz).rgb;
    vec3 colZ = texture(textureDiffuse, fs_in.position.xy).rgb;
    vec3 colXSpecular = texture(textureSpecular, fs_in.position.zy).rgb;
    vec3 colYSpecular = texture(textureSpecular, fs_in.position.xz).rgb;
    vec3 colZSpecular = texture(textureSpecular, fs_in.position.xy).rgb;
    vec3 colXNormal = texture(textureNormal, fs_in.position.zy).rgb;
    vec3 colYNormal = texture(textureNormal, fs_in.position.xz).rgb;
    vec3 colZNormal = texture(textureNormal, fs_in.position.xy).rgb;
    
    vec3 blendWeight = pow(abs(fs_in.normal), vec3(5));
    blendWeight /= dot(blendWeight, vec3(1));
    
    vec3 diffuseTex = colX * blendWeight.x + colY * blendWeight.y + colZ * blendWeight.z;
    vec3 specularTex = colXSpecular * blendWeight.x + colYSpecular * blendWeight.y + colZSpecular * blendWeight.z;
    vec3 normal = colXNormal * blendWeight.x + colYNormal * blendWeight.y + colZNormal * blendWeight.z;
    //vec3 normal = vec3(0, 0, 1);

    //vec3 diffuseTex = texture(textureDiffuse, fs_in.uv).rgb;
    //vec3 specularTex = texture(textureSpecular, fs_in.uv).rgb;
    //vec3 normal = texture(textureNormal, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    float shininess = 8;
    // ambient
    vec3 ambient = vec3(0.5, 0.7, 1) * 0.025 * diffuseTex;
    vec3 result = ambient;

    for (int i = 0; i < lightNum; i++)
    {
        if (i == usedLights)
            break;

        // CALCULATE LIGHTING

        // diffuse 
        vec3 lightDir = (typeL[i] == 0) ? fs_in.tangentLightDir[i] : normalize(fs_in.tangentFragPos - fs_in.tangentLightPos[i]);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = colourL[i].rgb * colourL[i].a * diff * diffuseTex;

        // specular
        vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
        vec3 reflectDir = reflect(lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = colourL[i].rgb * colourL[i].a * spec * specularTex;

        // spotlight (soft edges)
        if (typeL[i] == 1)
        {
            float theta = dot(lightDir, normalize(-fs_in.tangentLightDir[i]));
            float epsilon = (cutoffL[i] - outerCutoffL[i]);
            float intensity = clamp((theta - outerCutoffL[i]) / epsilon, 0.0, 1.0);
            diffuse *= intensity;
            specular *= intensity;
        }

        // attenuation
        if (typeL[i] != 0)
        {
            float distance = length(posL[i] - fs_in.position);
            float attenuation = 1.0 / (constantAtten + linearAtten * distance + quadraticAtten * (distance * distance));
            diffuse *= attenuation;
            specular *= attenuation;
        }

        // calculate shadow
        float shadow = ShadowCalculation(fs_in.positionLightSpace[i], i, normal);
        result += (1.0 - shadow) * (specular + diffuse);
    }

    colour = vec4(result, 1.0);
    //colour = vec4(vec3(fs_in.uv, 0.0), 1.0);
}