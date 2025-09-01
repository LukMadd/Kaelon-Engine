#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPos;

layout(push_constant) uniform PushConstants {
    mat4 model;
} pushConstants;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 lightDir;
    vec4 lightColorIntensity;
    vec4 cameraPos;
} ubo;

void main() {
    vec4 worldPos = pushConstants.model * vec4(inPosition, 1.0);
    fragPos = worldPos.xyz;

    fragNormal = normalize(mat3(transpose(inverse(pushConstants.model))) * inNormal);

    fragColor = inColor;
    fragTexCoord = inTexCoord;

    gl_Position = ubo.proj * ubo.view * worldPos;
}
