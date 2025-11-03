#include <cstdint>
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

        uint32_t newHashKey = componentStorage.getHash<First>();
        ((newHashKey |= componentStorage.getHash<Rest>()), ...);
        componentStorage.componentMap[newHashKey].push_back(e);
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

    template<typename... Components>
    std::vector<Entity> view(){
        uint32_t hash = (componentStorage.getHash<Components>() | ...);

        return componentStorage.componentMap[hash];
    }

    template<typename... Components, typename Func>
    void foreach(Func func){
        for(auto entity : view<Components...>()){
            func(getComponent<Components>(entity)...);
        }
    }

    private:
        std::vector<Entity> entities;
        ComponentStorage componentStorage;
};