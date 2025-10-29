#version 450
layout(push_constant) uniform Matrices {
    mat4 proj;
    mat4 view;
} pc;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = pc.proj * pc.view * vec4(inPos, 1.0);
    fragColor = inColor;
}
