#ifndef _LIGHTING_HPP
#define _LIGHTING_HPP

#include <glm/glm.hpp>
#include <vector>

namespace EngineRenderer{
    struct DirectionalLight{
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
    };

    struct PointLight{
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
        float radius;
    };

    class Lighting{
        public:
            void addDirectionalLight(const DirectionalLight& light) {directionalLights.push_back(light);}
            void addPointLight(const PointLight& light) {pointLights.push_back(light);}

            const std::vector<DirectionalLight> &getDirectionalLights() const {return directionalLights;}
            const std::vector<PointLight> &getPointLights() const {return pointLights;}

        private:
            std::vector<DirectionalLight> directionalLights;
            std::vector<PointLight> pointLights;
    };
}

#endif