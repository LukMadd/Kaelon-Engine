#include "CameraManager.hpp"

namespace EngineCamera{
    std::shared_ptr<Camera> CameraManager::getCurrentCamera(){
        return cameras[currentCameraIndex];
    }

    void CameraManager::pushCamera(std::shared_ptr<Camera> camera){
        if(!camera->is_initialized){
            camera->init();
        }
        camera->id = currentID;
        cameras.push_back(std::move(camera));
        currentID++;
    }

    void CameraManager::changeCamera(uint32_t cameraIndex){
        if(cameraIndex >= cameras.size() || cameraIndex < 0) return;

        currentCameraIndex = cameraIndex;
    }


}