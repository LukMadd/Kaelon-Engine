#include "Scene.hpp"
#include "BaseObjects.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineObject;

namespace EngineScene{
    Scene::Scene(){
        root = SceneNode();
        root.transform.position = glm::vec3(0.0f);
        root.transform.rotation = glm::quat(1.0f, 0, 0, 0);
        root.transform.scale    = glm::vec3(1.0f);
        root.transform.localMatrix = glm::mat4(1.0f);
        root.transform.worldMatrix = glm::mat4(1.0f);
    }

    //Will probably be removed later, right now just used because there is nothing to add scenes
    void Scene::initScene(bool whichScene, EngineRecourse::RecourseManager &recourseManager){
        glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
        if(whichScene){
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    auto mesh = recourseManager.loadMesh("models/Crate1.obj");
                    auto texture = recourseManager.loadTexture("textures/crate_1.jpg");
                    auto cube = std::make_unique<MeshObject>(pos, mesh, texture);
                    cube->modelMatrix = glm::scale(cube->modelMatrix, glm::vec3(0.5f));

                    SceneNode* node = new SceneNode();
                    node->object = cube.get();
                    node->transform.position = pos;
                    node->transform.scale = glm::vec3(0.5f);
                    node->transform.rotation = glm::quat(1.0f,0,0,0);

                    root.addChild(node);       
                    objects.push_back(std::move(cube));

                    pos.x+=1.75;
                }
                pos.x = 0;
                pos.z+=1.5;
                pos.y+=1.5;
        }
        } else{
            auto mesh = recourseManager.loadMesh("models/sphere.obj");
            auto texture = recourseManager.loadTexture("textures/viking_room.png");
            auto sphere = std::make_unique<MeshObject>(pos, mesh, texture);

            Object* objPtr = sphere.get();
            objects.push_back(std::move(sphere));

            SceneNode* node = new SceneNode();
            node->object = objPtr;
            node->transform.rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0,1,0));
            node->transform.scale = glm::vec3(1.0f);
            node->transform.position = glm::vec3(2,1,0);
            root.addChild(node);
        }

        root.update();
    }

    void Scene::cleanupObjects(){
        for(auto& obj : objects){
            obj->cleanup(device);
        }
        objects.clear();

        root.children.clear();
    }

    Scene Scene::createScene(){
        Scene scene;
        return scene;
    }

     void Scene::update(){
        root.update();
     }
}