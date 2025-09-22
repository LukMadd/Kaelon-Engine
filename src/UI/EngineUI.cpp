#include "EngineUI.hpp"
#include "Object.hpp"
#include "RendererGlobals.hpp"
#include "Scene.hpp"
#include "imgui.h"

namespace EngineUI{
    void EngineUI::drawMainLayout(EngineScene::SceneManager *sceneManager){
            if(ImGui::BeginMenuBar()){
            if(ImGui::BeginMenu("Scene")){
                ImGui::Checkbox("Scene Hierarchy", &m_showSceneHierarchyWindow);
                ImGui::Checkbox("Object Inspector", &m_showObjectInspector);
                ImGui::Checkbox("Camera Inspector", &m_showCameraWindow);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Engine")){
                ImGui::Checkbox("Resources", &m_showResourceWindow);
                ImGui::Checkbox("Render Stats", &m_showRenderStats);
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Actions")){
                if(ImGui::Button("Add Scene")){
                    sceneManager->addDefaultScene();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
    }

    void EngineUI::drawSceneHierarchy(EngineScene::Scene *scene){
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
                    for(auto &camera : scene->cameraManager.getCameras()){
                        ImGui::PushID(camera.get());
                        bool is_camera_selected = (selectedCamera == camera.get());
                        if(ImGui::Selectable(camera->name.c_str(), is_camera_selected)){
                            if(selectedCamera) selectedCamera->selected = false;
                            selectedCamera = camera.get();
                            selectedCamera->selected = true;

                            scene->cameraManager.changeCamera(cameraIndex);

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

    void EngineUI::drawCameraInspector(){
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

    void EngineUI::drawRecourses(EngineResource::ResourceManager *recourseManager){
        if(m_showResourceWindow){
            ImGui::Begin("Recourses");

            //Can optimize this by finding textures and models by pushing into the vectors if the cache type name is the same 
            //as the desired(recourse.second.type().name() ==) though it will require refactoring if there is a change of stored type 
            std::vector<std::string> recourseTextures;
            std::vector<std::string> recourseModels;

            //Get the textures & models from recource manager and put them in the vectors above
            for(auto &recourse : recourseManager->getCache()){
               
                if(recourse.first.find("textures/") != std::string::npos){
                    recourseTextures.push_back(recourse.first);
                }

                if(recourse.first.find("models/") != std::string::npos){
                    recourseModels.push_back(recourse.first);
                }
            }

            //Put the textures & models in a tree node for the UI
            if(ImGui::TreeNode("Textures")){
                for(int i = 0; i < recourseTextures.size(); i++){
                    ImGui::Text("%s", recourseTextures[i].c_str());
                }
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("Models")){
                for(int i = 0; i < recourseModels.size(); i++){
                    ImGui::Text("%s", recourseModels[i].c_str());
                }
                ImGui::TreePop();
            }

            ImGui::End();
        }
    }

    void EngineUI::drawRenderStats(EngineScene::Scene *scene, float fps){
        if(m_showRenderStats){
            int triangleCount = 0;
            for(auto &object : scene->objects){
                if(object->mesh){
                    triangleCount+=object->mesh->indexCount / 3;
                }
            }

            ImGui::Begin("Render Stats");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Frame Time: %.2fms", 1000.f / fps);
            ImGui::Text("Draw Calls: %d", drawCallCountLastFrame);
            ImGui::Text("Triangle Count: %d", triangleCount);
            ImGui::Checkbox("Wireframe Mode", &wireFrameModeEnabled);
            ImGui::End();
        }
    }

}