#include "Camera.hpp"
#include "Action.hpp"
#include "Input.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace EngineInput;

namespace EngineCamera{
    Camera::Camera(){
        position = glm::vec3(3.0f, 1.0f, 0.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 target = glm::vec3((0.0f));
        glm::vec direction = glm::normalize(target - position);

        yaw = glm::degrees(atan2(direction.z, direction.x ));
        pitch = glm::degrees(asin(direction.y));

        front = direction;
    }

    void Camera::moveUp(float velocity){
        position += up * velocity;
    }

    void Camera::moveDown(float velocity){
        position -= up * velocity;
    }

    void Camera::moveForward(float velocity){
        position += front * velocity;
    }

    void Camera::moveBackward(float velocity){
        position -= front * velocity;
    }

    void Camera::moveLeft(float velocity){
        position -= glm::normalize(glm::cross(front, up)) * velocity;
    }

    void Camera::moveRight(float velocity){
        position += glm::normalize(glm::cross(front, up)) * velocity;
    }

    void Camera::updateYawAndPitch(){
        Input &input = Input::get();

        glm::vec2 offsets = input.getOffset();

        yaw += offsets[0] * sensitivity;
        pitch += offsets[1] * sensitivity;

        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;

        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(front);

        input.xOffset = 0.0;
        input.yOffset = 0.0;
    }

    void Camera::updateCameraPosition(float deltaTime, ActionManager &actionManager){
        velocity = speed * deltaTime;

        if(actionManager.isActionActive(Action::PLAYER_MOVE_UP)){
            moveUp(velocity);
        }
        if(actionManager.isActionActive(Action::PLAYER_MOVE_DOWN)){
            moveDown(velocity);
        }
        if(actionManager.isActionActive(Action::PLAYER_MOVE_FORWARD)){
            moveForward(velocity);
        } 
        if(actionManager.isActionActive(Action::PLAYER_MOVE_BACKWARD)){
            moveBackward(velocity);
        } 
        if(actionManager.isActionActive(Action::PLAYER_MOVE_LEFT)){
            moveLeft(velocity);
        } 
        if(actionManager.isActionActive(Action::PLAYER_MOVE_RIGHT)){
            moveRight(velocity);
        }

        updateYawAndPitch();
    }

    glm::mat4 Camera::getViewMatrix(){
        return glm::lookAt(position, position + front, up);
    }
}