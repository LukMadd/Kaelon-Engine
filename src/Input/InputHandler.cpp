#include "Input/InputHandler.hpp"
#include "Input/InputBindings.hpp"
#include "Scene/SceneManager.hpp"
#include "Input/Action.hpp"
#include "Input/Input.hpp"

namespace EngineInput{
    bool InputHandler::isPressed(Key key){
        return Input::get().isKeyPressed(static_cast<int>(key));
    }

    void InputHandler::update(GLFWwindow* window, ActionManager &actionManager, EngineScene::SceneManager &sceneManager){
        is_camera_locked_action_active = false;
        for(auto& [action, binding] : inputBindings){
            if(actionManager.isActionActive(action)){
                binding();
            }
        }
        was_camera_locked_action_active = is_camera_locked_action_active;
    }
};
