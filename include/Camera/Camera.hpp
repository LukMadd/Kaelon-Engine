#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/glm.hpp>
#include "Action.hpp"

#include <string>

using namespace EngineInput;

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

            glm::vec3& getFront(){return front;}
            glm::vec3& getUp(){return up;}

        private:
            glm::vec3 front;
            glm::vec3 up;
            float fov = 35.0f;
            float speed = 10.0f;
            float velocity;
            float yaw = 0.0f;
            float pitch = 0.0f;
            float sensitivity = 0.1f;

            float lastYawUpdate = 0.0f; //To allow for yaw & pitch editing while the camera is locked
            float lastPitchUpdate = 0.0f;

            void moveUp(float velocity);
            void moveDown(float velocity);
            void moveForward(float velocity);
            void moveBackward(float velocity);
            void moveLeft(float velocity);
            void moveRight(float velocity);

        public:
            void updateCameraPosition(float deltaTime, ActionManager &actionManager, bool is_scene_immersed);

            void updateYawAndPitch();

            glm::mat4 getViewMatrix();
    };
}

#endif