#zwrite off
#cull off
#blend on
#blendsrc SrcAlpha
#blenddest OneMinusSrcAlpha
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
out vec4 colour;
out float gl_FragDepth;

uniform sampler2D blitTexture;
uniform sampler2D depthTexture;

uniform sampler2D previousFrame;
uniform sampler2D previousFrameDepth;

uniform mat4x4 previousCamMatrix;
uniform mat4x4 camMatrixInv;

uniform float nearPlane;
uniform float farPlane;

uniform vec3 cameraJitter;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; // Back to NDC 
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main() {
	float depth = texture(depthTexture, UV).r;

	// converts UV and depth to worldSpacePosition
	vec4 clipSpacePos = vec4(UV * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
	vec4 worldSpacePos = camMatrixInv * clipSpacePos;
	worldSpacePos /= worldSpacePos.w;
	worldSpacePos.xyz += cameraJitter;
	
	// converts worldSpacePosition into previousCamMatrix space
	vec4 previousCamSpace = previousCamMatrix * worldSpacePos;
	vec3 previousCamPos = previousCamSpace.xyz / previousCamSpace.w;
	vec2 newUVs = previousCamPos.xy * 0.5 + 0.5;
	float oldDepth = LinearizeDepth(texture(previousFrameDepth, newUVs).r);
	if (clamp(newUVs, 0, 1) == newUVs)
	{
		colour = 
			(vec4(texture(previousFrame, newUVs).rgb, 1) * 0.5 +
			texture(blitTexture, UV)) * 0.6666666666666;
	}
	else
	{
		colour = texture(blitTexture, UV);
	}

	gl_FragDepth = depth;
	//colour = vec4(abs(texture(blitTexture, UV).rgb - texture(previousFrame, UV).rgb), 1);
}