#ifndef _SCENE_HPP
#define _SCENE_HPP

#include "CameraManager.hpp"
#include "Object.hpp"
#include "RecourseManager.hpp"

using namespace EngineObject;

namespace EngineScene{
    class Scene{
        public:
            Scene(const std::string &name, int index);
            ~Scene() = default;

            EngineCamera::CameraManager cameraManager;

            int index;
            std::string name;
            std::vector<std::unique_ptr<EngineObject::Object>> objects; 
            std::vector<EngineObject::Object*> newObjects;
            std::vector<VkDescriptorSet> descriptorSets;
            SceneNode root;

            bool areObjectsInitialized = false;
            bool areDescriptorSetsInitialized = false;

            bool selected = false;

            static std::unique_ptr<Scene> createScene(int id, const std::string &name);
            void initBaseScene(EngineResource::ResourceManager &resourceManager);

            void pushObject(std::unique_ptr<Object> object);

            void addDefaultObject();

            void removeObject(Object *object);

            int getIndex() const {return index;}
            std::string getName() const {return name;}

            std::vector<std::shared_ptr<Texture>> getSceneTextures();

            void update();

            void cleanupObjects();
    };
}

#endif