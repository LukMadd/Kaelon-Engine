#include "InputHandler.hpp"
#include "Action.hpp"
#include "Input.hpp"
#include "Window.hpp"

namespace EngineInput{
    bool InputHandler::isPressed(Key key){
        return Input::get().isKeyPressed(static_cast<int>(key));
    }

    void InputHandler::update(GLFWwindow* window, ActionManager &actionManager){
        if(actionManager.isActionActive(Action::USER_ESCAPE_PROGRAM)){
            if(Engine::isFullscreen){
                toggleFullscreen(window);
            } else{
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        if(actionManager.isActionActive(Action::USER_FULL_SCREEN)){
            if(Engine::isFullscreen == false){
                toggleFullscreen(window);
            }
        }
        if(actionManager.isActionActive(Action::USER_HIDE_CURSOR)){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Input::get().resetMouse();
        }
    }
};