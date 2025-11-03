#include "Components.hpp"
#include <cstdint>
#include <sys/types.h>
#include <unordered_map>
#include "vector"

//Component Accessor 
template<typename T>
struct ComponentAccessor;

struct ComponentStorage{
    std::unordered_map<uint32_t, std::vector<Entity>> componentMap;

    std::vector<TransformComponent> transforms;
    std::unordered_map<Entity, size_t> transformIndices;

    std::vector<MeshComponent> meshes;
    std::unordered_map<Entity, size_t> meshIndices;

    std::vector<MaterialComponent> materials;
    std::unordered_map<Entity, size_t> materialIndices;

    std::vector<PhysicsComponent> physics;
    std::unordered_map<Entity, size_t> physicsIndices;

    std::vector<BoundingBoxComponent> boundingBoxes;
    std::unordered_map<Entity, size_t> boundingBoxIndices;

    std::vector<SpatialPartitioningComponent> spatials;
    std::unordered_map<Entity, size_t> spatialIndices;

    std::vector<RenderableComponent> renderables;
    std::unordered_map<Entity, size_t> renderableIndices;

    std::vector<SceneNodeComponent> sceneNodes;
    std::unordered_map<Entity, size_t> sceneNodeIndices;

    std::vector<MetadataComponent> metadata;
    std::unordered_map<Entity, size_t> metadataIndices;

    template<typename T, typename... Args>
    void addComponent(Entity e, Args&&... args){
        auto &vec = ComponentAccessor<T>::getVector(*this);
        auto &map = ComponentAccessor<T>::getMap(*this);

        vec.emplace_back(std::forward<Args>(args)...);
        map[e] = vec.size() - 1;
    }

    template<typename T>
    auto& getIndices(){
        return ComponentAccessor<T>::getMap(*this);
    }

    template<typename T>
    auto &getVector(){
        return ComponentAccessor<T>::getVector(*this);
    }

    template<typename... Components>
    uint32_t getHash(){
       return (ComponentAccessor<Components>::hash | ...);
    }
};


//Specializations
template<>
struct ComponentAccessor<TransformComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.transforms;}
    static auto &getMap(ComponentStorage &storage){return storage.transformIndices;}
    static inline uint32_t hash = 1u << 0;
};


template<>
struct ComponentAccessor<MeshComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.meshes;}
    static auto &getMap(ComponentStorage &storage){return storage.meshIndices;}
    static inline uint32_t hash = 1u << 1;
};

template<>
struct ComponentAccessor<PhysicsComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.physics;}
    static auto &getMap(ComponentStorage &storage){return storage.physicsIndices;}
    static inline uint32_t hash = 1u << 3;
};

template<>
struct ComponentAccessor<BoundingBoxComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.boundingBoxes;}
    static auto &getMap(ComponentStorage &storage){return storage.boundingBoxIndices;}
    static inline uint32_t hash = 1u << 4;
};

template<>
struct ComponentAccessor<SpatialPartitioningComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.spatials;}
    static auto &getMap(ComponentStorage &storage){return storage.spatialIndices;}
    static inline uint32_t hash = 1u << 5;
};

template<>
struct ComponentAccessor<RenderableComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.renderables;}
    static auto &getMap(ComponentStorage &storage){return storage.renderableIndices;}
    static inline uint32_t hash = 1u << 6;
};

template<>
struct ComponentAccessor<SceneNodeComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.sceneNodes;}
    static auto &getMap(ComponentStorage &storage){return storage.sceneNodeIndices;}
    static inline uint32_t hash = 1u << 7;
};

template<>
struct ComponentAccessor<MetadataComponent>{
    static auto &getVector(ComponentStorage &storage){return storage.metadata;}
    static auto &getMap(ComponentStorage &storage){return storage.metadataIndices;}
    static inline uint32_t hash = 1u << 8;
};
