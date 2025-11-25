#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "ECS/Components.hpp"
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <string>

namespace EngineInput {
    class ActionManager;
}

namespace EngineCamera{   
    class Camera{
        public:
            void init();
            glm::vec3 position;
            
            bool is_initialized = false;
            bool selected = false;

            int id;

            std::string name;

            float& getFov(){return fov;}
            float& getSpeed(){return speed;}
            float& getYaw(){return yaw;}
            float& getPitch(){return pitch;}
            float& getSensitivity(){return sensitivity;}

            std::array<glm::vec4, 2> transformRay(glm::vec4 rayStartNDC, glm::vec4 rayEndNDC);

            glm::vec3& getFront(){return front;}
            glm::vec3& getUp(){return up;}

            void giveExtent(VkExtent2D extent){swapChainExtent = extent;}

        private:
            glm::vec3 front;
            glm::vec3 up;
            glm::mat4 projection;
            glm::mat4 view;
            float fov = 35.0f;
            float speed = 10.0f;
            float velocity;
            float yaw = 0.0f;
            float pitch = 0.0f;
            float sensitivity = 0.1f;

            float lastYawUpdate = 0.0f; //To allow for yaw & pitch editing while the camera is locked
            float lastPitchUpdate = 0.0f;

            Entity target_entity = nullEntity;
            TransformComponent* target_transform = nullptr;
            glm::vec3 target_offset = {FLT_MAX, FLT_MAX, FLT_MAX};

            VkExtent2D swapChainExtent;

            void moveUp(float velocity);
            void moveDown(float velocity);
            void moveForward(float velocity);
            void moveBackward(float velocity);
            void moveLeft(float velocity);
            void moveRight(float velocity);

            void setCameraDefault();

        public:
            bool isFixed = false;

            void updateCamera(float deltaTime, EngineInput::ActionManager &actionManager, bool is_scene_immersed);

            void updateYawAndPitch();

            glm::mat4 getViewMatrix() {return view;};

           inline  glm::mat4 getProjection() {return projection;}

            void setTarget(Entity e, TransformComponent* transform){
                target_entity = e;
                target_transform = transform;
            }
            void setTargetOffset(glm::vec3 offset){target_offset = offset;}

            Entity getTarget(){return target_entity;}
            glm::vec3& getTargetOffset(){return target_offset;}

            void resetTarget(){
                target_entity = nullEntity;
                target_transform = nullptr;
                target_offset = {FLT_MAX, FLT_MAX, FLT_MAX};;
            }


           void resetCamera();
    };
}

#endif