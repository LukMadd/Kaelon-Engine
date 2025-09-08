#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec4 lightDir;
    vec4 lightColorIntensity;
    vec4 cameraPos;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(-ubo.lightDir.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * ubo.lightColorIntensity.rgb * ubo.lightColorIntensity.w * 0.75;
    vec3 viewDir = normalize(ubo.cameraPos.xyz - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = 0.75 * spec * ubo.lightColorIntensity.rgb * ubo.lightColorIntensity.w;
    vec3 texColor = texture(texSampler, fragTexCoord).rgb;
    vec3 ambient = 0.5 * texColor;
    
    vec3 linearColor = ambient + diffuse + specular;
    vec3 finalColor = min(linearColor, vec3(1.0));

    outColor = vec4(finalColor, 1.0);
}