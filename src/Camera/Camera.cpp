#include "Camera/Camera.hpp"
#include "Input/Action.hpp"
#include "Input/Input.hpp"
#include "Input/InputBindings.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

constexpr float RENDER_DISTANCE = 100.0f;

using namespace EngineInput;

namespace EngineCamera{
    void Camera::setCameraDefault(){
        projection = glm::perspective(glm::radians(fov), 0.0f, 0.1f, RENDER_DISTANCE);
        position = glm::vec3(-0.2f, 11.5f, -33.5f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 target = glm::vec3((0.0f));
        glm::vec3 direction = glm::normalize(target - position);

        yaw = glm::degrees(atan2(direction.z, direction.x ));
        pitch = glm::degrees(asin(direction.y));

        front = direction;
    }

    void Camera::init(){
        setCameraDefault();

        name = "Generic_Camera";
        is_initialized = true;
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

        lastYawUpdate = yaw;
        lastPitchUpdate = pitch;
    }

    void Camera::updateCamera(float deltaTime, EngineInput::ActionManager &actionManager){
        projection = glm::perspective(glm::radians(fov), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, RENDER_DISTANCE);

        velocity = speed * deltaTime;

        view = glm::lookAt(position, position + front, up);

        if(!isFixed && !is_camera_locked || yaw != lastYawUpdate  || pitch != lastPitchUpdate){
            updateYawAndPitch();
        }
    }

    void Camera::resetCamera(){
        setCameraDefault();
    }

    std::array<glm::vec4, 2> Camera::transformRay(glm::vec4 rayStartNDC, glm::vec4 rayEndNDC){
        glm::mat4 invVP = glm::inverse(projection * view);

        glm::vec4 rayStartWorld = invVP * rayStartNDC;
        rayStartWorld/=rayStartWorld.w;

        glm::vec4 rayEndWorld = invVP * rayEndNDC;
        rayEndWorld/=rayEndWorld.w;

        std::array<glm::vec4, 2> result;
        result[0] = rayStartWorld;
        result[1] = rayEndWorld;

        return result;
    }
}
