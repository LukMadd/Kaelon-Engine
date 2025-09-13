#include "Input.hpp"
#include "imgui_impl_glfw.h"

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

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
            Input::KeyCallBack(window, key, scancode, action, mods);
        });

        glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int c){
            ImGui_ImplGlfw_CharCallback(window, c);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
            ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
            Input::MouseButtonCallBack(window, button, action, mods);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset){
            ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
        });

         
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
        ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

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