#include "ObjectRegistry.hpp"

namespace EngineObject{
    void ObjectRegistry::registerType(const std::string& type, ObjectFactory factory){
        factories[type] = factory;
    }

    Object* ObjectRegistry::create(const std::string& type, const nlohmann::json& data){
        return factories.at(type)(data);
    }
}