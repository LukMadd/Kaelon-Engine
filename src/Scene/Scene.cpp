#include "Scene.hpp"
#include "BaseObjects.hpp"
#include "RendererGlobals.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineObject;

namespace EngineScene{
    //Will probably be removed later, right now just used because there is nothing to add scenes
    void Scene::initScene(bool whichScene){
        if(whichScene){
            glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
            for(int i = 0; i < 10; i++){
                for(int j = 0; j < 10; j++){
                    auto cube = std::make_unique<MeshObject>(pos, "models/Crate1.obj", "textures/crate_1.jpg");
                    cube->modelMatrix = glm::scale(cube->modelMatrix, glm::vec3(0.5f));

                    objects.push_back(std::move(cube));

                    pos.x+=1.75;
                }
                pos.x = 0;
                pos.z+=1.5;
                pos.y+=1.5;
        }
    } else{
        auto mesh = std::make_unique<MeshObject>(glm::vec3(0.0f , 0.0f, 0.0f), "models/viking_room.obj", "textures/viking_room.png");
        objects.push_back(std::move(mesh));
    }
    }

    void Scene::cleanupObjects(){
        for(auto& obj : objects){
            obj->cleanup(device);
        }
        objects.clear();
    }

    Scene Scene::createScene(){
        Scene scene;
        return scene;
    }
}