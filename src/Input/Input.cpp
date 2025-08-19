#include "Input.hpp"

using namespace Engine;

namespace EngineInput{
    std::unordered_map<int, bool> Input::keyStates;
    std::unordered_map<int, bool> Input::mouseButtonStates;

    void Input::init(GLFWwindow* windowRef){
        window = windowRef;
    }

    void Input::KeyCallBack(GLFWwindow *window, int key, int scanCode, int action, int mods){
        keyStates[key] = (action != GLFW_RELEASE);
    }

    void Input::MouseButtonCallBack(GLFWwindow *window, int button, int action, int mods){
        mouseButtonStates[button] = (action != GLFW_RELEASE);
    }

    void Input::setCallBacks(){
        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, KeyCallBack);
        glfwSetMouseButtonCallback(window, MouseButtonCallBack);
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
        auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));

        if(input->firstMouse || input->ignoreMouseDelta){
            input->lastX = xpos;
            input->lastY = ypos;
            input->firstMouse = false;
            input->ignoreMouseDelta = false;
            input->xOffset = 0.0;
            input->yOffset = 0.0;
            return;
        }

        input->xOffset = xpos - input->lastX;
        input->yOffset = input->lastY - ypos;

        input->lastX = xpos;
        input->lastY = ypos;
        });
    }

    glm::vec2 Input::getOffset(){
        glm::vec2 offset{xOffset, yOffset};
        xOffset = 0.0;
        yOffset = 0.0;
        return offset;
    }

    void Input::resetMouse(){
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        lastX = mouseX;
        lastY = mouseY;
        xOffset = 0.0;
        yOffset = 0.0;
        ignoreMouseDelta = true;
    }

    bool Input::isKeyPressed(int key){
        return keyStates[key];
    }

    bool Input::isButtonPressed(int button){
        return mouseButtonStates[button];
    }
}