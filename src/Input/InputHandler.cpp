#include "InputHandler.hpp"
#include "SceneManager.hpp"
#include "Action.hpp"
#include "Input.hpp"
#include "Window.hpp"

namespace EngineInput{
    bool InputHandler::isPressed(Key key){
        return Input::get().isKeyPressed(static_cast<int>(key));
    }

    void InputHandler::update(GLFWwindow* window, ActionManager &actionManager, EngineScene::SceneManager &sceneManager){
        if(actionManager.isActionActive(Action::USER_ESCAPE_PROGRAM)){
            if(Engine::isFullscreen){
                toggleFullscreen(window);
            } else{
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                Input::get().resetMouse();
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
        if(actionManager.isActionActive(Action::DEVELOPER_CHANGE_SCENE_PLUS)){
            sceneManager.changeScenes(1);
        } 
        if(actionManager.isActionActive(Action::DEVELOPER_CHANGE_SCENE_MINUS)){
            sceneManager.changeScenes(-1);
        }

        //Logic prevents multiple switching in one press(Disables holding down the key)
        bool is_immersed_action_active = actionManager.isActionActive(Action::DEVELOPER_SCENE_IMMERSED);
        if(is_immersed_action_active && !was_immersed_action_active){
            is_scene_immersed = !is_scene_immersed;
        }

        was_immersed_action_active = is_immersed_action_active;
    }
};