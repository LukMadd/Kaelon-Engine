#include "EngineUI.hpp"
#include "Object.hpp"
#include "Scene.hpp"
#include "imgui.h"

namespace EngineUI{
    void EngineUI::drawMainLayout(){
        if(ImGui::BeginMenuBar()){
            ImGui::Checkbox("Scene Hierarchy", &m_showSceneHierarchyWindow);
            ImGui::Checkbox("Object Inspector", &m_showObjectInspector);
            ImGui::Checkbox("Camera", &m_showCameraWindow);
            ImGui::Checkbox("Recources", &m_showResourceWindow);
            ImGui::Checkbox("Render Stats", &m_showRenderStats);
            ImGui::EndMenuBar();
        }
    }

    void EngineUI::drawSceneHierarchy(EngineScene::Scene *scene, EngineCamera::CameraManager *cameraManager){
        if(m_showSceneHierarchyWindow){
            ImGui::Begin("Scene Hierarchy");
            const char* object_preview_text = selectedObject ? selectedObject->name.c_str() : "Select an object";
            if(ImGui::BeginCombo("Objects", object_preview_text)){
                for(auto &object : scene->objects){
                    ImGui::PushID(object.get());
                    bool is_object_selected = (selectedObject == object.get());
                    if(ImGui::Selectable(object->name.c_str(), is_object_selected)){
                        if(selectedObject) selectedObject->selected = false;
                        selectedObject = object.get();
                        selectedObject->selected = true;

                        if(!m_showObjectInspector){
                            m_showObjectInspector = true;
                        }
                    }
                    if(is_object_selected){
                        ImGui::SetItemDefaultFocus();
                    }
                    ImGui::PopID();
                }
                ImGui::EndCombo();
            }

                uint32_t cameraIndex = 0;
                const char* camera_preview_text = selectedCamera ? selectedCamera->name.c_str() : "Select an camera";
                if(ImGui::BeginCombo("Cameras", camera_preview_text)){
                    for(auto &camera : cameraManager->getCameras()){
                        ImGui::PushID(camera.get());
                        bool is_camera_selected = (selectedCamera == camera.get());
                        if(ImGui::Selectable(camera->name.c_str(), is_camera_selected)){
                            if(selectedCamera) selectedCamera->selected = false;
                            selectedCamera = camera.get();
                            selectedCamera->selected = true;

                            cameraManager->changeCamera(cameraIndex);

                            if(!m_showCameraWindow){
                                m_showCameraWindow = true;
                            }
                        }
                        if(is_camera_selected){
                            ImGui::SetItemDefaultFocus();
                        }
                        ImGui::PopID();
                        cameraIndex++;
                    }
                    ImGui::EndCombo();
                }
            
            ImGui::End();
        }
    }

    void EngineUI::drawObjectInspector(){
        if(m_showObjectInspector && selectedObject){
            ImGui::Begin("Object Inspector");

            ImGui::Text("UUID: %s", selectedObject->uuid.c_str());

            ImGui::Text("Name: %s", selectedObject->name.c_str());
            if(ImGui::TreeNode("Position")){
                ImGui::InputFloat("X: ", &selectedObject->node->transform.position.x);
                ImGui::InputFloat("Y: ", &selectedObject->node->transform.position.y);
                ImGui::InputFloat("Z: ", &selectedObject->node->transform.position.z);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("Rotation")){
                ImGui::InputFloat("X: ", &selectedObject->node->transform.rotation.x);
                ImGui::InputFloat("Y: ", &selectedObject->node->transform.rotation.y);
                ImGui::InputFloat("Z: ", &selectedObject->node->transform.rotation.z);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("Scale")){
                ImGui::InputFloat("X: ", &selectedObject->node->transform.scale.x);
                ImGui::InputFloat("Y: ", &selectedObject->node->transform.scale.y);
                ImGui::InputFloat("Z: ", &selectedObject->node->transform.scale.z);
                ImGui::TreePop();
            }

            ImGui::End();
        }
    }

    void EngineUI::drawCamera(){
        if(m_showCameraWindow && selectedCamera){
            ImGui::Begin("Camera Inspector");

            ImGui::Text("Name: %s", selectedCamera->name.c_str());
            if(ImGui::TreeNode("Position")){
                ImGui::InputFloat("X: ", &selectedCamera->position.x);
                ImGui::InputFloat("Y: ", &selectedCamera->position.y);
                ImGui::InputFloat("Z: ", &selectedCamera->position.z);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("Rotation")){
                ImGui::InputFloat("Yaw: ", &selectedCamera->getYaw());
                ImGui::InputFloat("Pitch: ", &selectedCamera->getPitch());
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("Attributes")){
                ImGui::InputFloat("FOV: ", &selectedCamera->getFov());
                ImGui::InputFloat("Speed: ", &selectedCamera->getSpeed());
                ImGui::InputFloat("Sensitivity: ", &selectedCamera->getSensitivity());
                ImGui::TreePop();
            }

            ImGui::End();
        }
    }


}