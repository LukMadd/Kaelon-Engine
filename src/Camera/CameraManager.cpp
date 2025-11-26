#include "Camera/CameraManager.hpp"

namespace EngineCamera{
    std::shared_ptr<Camera> CameraManager::getCurrentCamera(){
        checkIfCamerasEmpty();
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

        hasCameraChanged = true;
    }

    //Prevents putting the same code in multiple different places
    void CameraManager::checkIfCamerasEmpty(){
        if(getCameras().empty()){
            EngineCamera::Camera camera;
            pushCamera(std::make_shared<EngineCamera::Camera>(camera));
        }
    }


}