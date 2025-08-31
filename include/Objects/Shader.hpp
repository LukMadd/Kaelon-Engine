#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

struct Shader{
    std::string vertShader = ("shaders/base_vert_shader.spv");
    std::string fragShader = ("shaders/base_frag_shader.spv");
};

#endif