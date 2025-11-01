#include <sys/types.h>
#include <vector>
#include "Components.hpp"
#include "ComponentStorage.hpp"

struct ECS{
    void addEntity(Entity e){entities.push_back(e);}

    template<typename T, typename... Args>
    void addComponent(Entity e, Args&&... args){
        componentStorage.addComponent<T>(e, std::forward<Args>(args)...);
    }

    template<typename First, typename... Rest>
    void addComponents(Entity e){
        addComponent<First>(e);
        (addComponent<Rest>(e), ...);
    }

    template<typename Component>
    auto &getComponent(Entity e){
        auto &map = componentStorage.getIndices<Component>();
        auto &vec = componentStorage.getVector<Component>();

        return vec[map[e]];
    }

    template<typename... Components>
    auto getComponents(Entity e) {
        return std::tie(getComponent<Components>(e)...);
    }

    template<typename T>
    bool hasComponent(Entity e){
        auto &map = componentStorage.getIndices<T>();
        return map.find(e) != map.end();
    }

    template<typename First, typename... Rest>
    std::vector<Entity> view(){
        std::vector<Entity> results;

        auto& baseIndices = componentStorage.getIndices<First>();
        
        for(const auto& [entity, _] : baseIndices){
            if((hasComponent<Rest>(entity) && ...)){
                results.push_back(entity);
            }
        }
        return results;
    }

    private:
        std::vector<Entity> entities;
        ComponentStorage componentStorage;
};