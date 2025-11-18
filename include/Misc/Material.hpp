#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "Texture.hpp"

struct Material{
    public:
        Material() = default;
        ~Material() = default;

        void addTexture(std::shared_ptr<Texture> texture) {textures.push_back(texture);}
        std::vector<std::shared_ptr<Texture>> &getTextures() {return textures;}

        void setShader(Shader &shader) {this->shader = shader;}
        const Shader &getShader() const {return shader;}

        void setBaseColor(const glm::vec4 &baseColor) {this->baseColor = baseColor;}
        const glm::vec4 &getBaseColor() const {return baseColor;}

        void setRoughness(float roughness) {this->roughness = roughness;}
        const float &getRoughness() const {return roughness;}

        void setMetallic(float metallic) {this->metallic = metallic;}
        const float getMetallic() const {return metallic;}

        void setAlbedo(float albedo) {this->albedo = albedo;}
        const float getAlbedo() const {return albedo;}

        void addPendingTexture(std::string pendingTexture){this->pendingTexturePaths.push_back(pendingTexture);}
        std::vector<std::string>& getPendingTextures(){return this->pendingTexturePaths;}

    private:
        Shader shader;
        std::vector<std::shared_ptr<Texture>> textures;

        glm::vec4 baseColor{1.0f, 1.0f, 1.0f, 1.0f};
        float roughness{1.0f};
        float metallic{0.0f};
        float albedo{0.0f};

        std::vector<std::string> pendingTexturePaths;
};


#endif