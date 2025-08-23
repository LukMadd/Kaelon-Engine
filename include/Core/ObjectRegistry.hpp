#include <functional>
#include <unordered_map>
#include <string>

#include "Object.hpp"
#include "nlohmann/json.hpp"

using namespace EngineScene;

namespace EngineObject{
    class ObjectRegistry{
        public:
            using ObjectFactory = std::function<Object*(const nlohmann::json&)>;

            static ObjectRegistry &get(){
                static ObjectRegistry instance;
                return instance;
            }

            void registerType(const std::string& type, ObjectFactory factory);

            Object* create(const std::string& type, const nlohmann::json& data);

        private:
            std::unordered_map<std::string, ObjectFactory> factories;

            ObjectRegistry() = default;

    };
}