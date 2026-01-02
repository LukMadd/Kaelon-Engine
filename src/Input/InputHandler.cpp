#include "Input/InputHandler.hpp"
#include "Scene/SceneManager.hpp"
#include "Input/Action.hpp"
#include "Input/Input.hpp"

namespace EngineInput{
    bool InputHandler::isPressed(Key key){
        return Input::get().isKeyPressed(static_cast<int>(key));
    }

    void InputHandler::update(GLFWwindow* window, ActionManager &actionManager, EngineScene::SceneManager &sceneManager){
        for(auto& [action, binding] : inputBindings){
            if(actionManager.isActionActive(action)){
                binding();
            }
        }
    }
};