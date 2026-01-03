#include "Input/Action.hpp"
#include "Input/InputHandler.hpp"

namespace EngineInput{    
    void ActionManager::bind(int action, Key key){
        bindedActions[action] = key;
    }

    bool ActionManager::isActionActive(int action){
        return inputHandler->isPressed(bindedActions[action]);
    }

    void ActionManager::setupDeveloperBindings(){
        bind(Developer::TOGGLE_CAMERA_LOCK, Key::KEY_PERIOD);
        bind(Developer::CAMERA_FORWARD, Key::KEY_W);
        bind(Developer::CAMERA_BACKWARD, Key::KEY_S);
        bind(Developer::CAMERA_LEFT, Key::KEY_A);
        bind(Developer::CAMERA_RIGHT, Key::KEY_D);
        bind(Developer::CAMERA_UP, Key::KEY_SPACE);
        bind(Developer::CAMERA_DOWN, Key::KEY_TAB);
        bind(Developer::EXIT_FULL_SCREEN, Key::KEY_ESCAPE);
        bind(Developer::ENTER_FULL_SCREEN, Key::KEY_F10);
        bind(Developer::HIDE_CURSOR, Key::KEY_SHIFT);
        bind(Developer::SHOW_CURSOR, Key::KEY_GRAVE_ACCENT);
    }
}
