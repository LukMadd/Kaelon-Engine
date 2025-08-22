#include "Action.hpp"
#include "InputHandler.hpp"

namespace EngineInput{    
    void ActionManager::bind(Action action, Key key){
        bindedActions[action] = key;
    }

    bool ActionManager::isActionActive(Action action){
        return inputHandler->isPressed(bindedActions[action]);
    }

    void ActionManager::setupBindings(){
        bind(Action::PLAYER_MOVE_UP, Key::KEY_ARROW_UP);
        bind(Action::PLAYER_MOVE_DOWN, Key::KEY_ARROW_DOWN);
        bind(Action::PLAYER_MOVE_FORWARD, Key::KEY_W);
        bind(Action::PLAYER_MOVE_BACKWARD, Key::KEY_S);
        bind(Action::PLAYER_MOVE_LEFT, Key::KEY_A);
        bind(Action::PLAYER_MOVE_RIGHT, Key::KEY_D);
        bind(Action::PLAYER_JUMP, Key::KEY_SPACE);
        bind(Action::PLAYER_CROUCH, Key::KEY_TAB);
        bind(Action::USER_ESCAPE_PROGRAM, Key::KEY_ESCAPE);
        bind(Action::USER_FULL_SCREEN, Key::KEY_F10);
        bind(Action::USER_HIDE_CURSOR, Key::KEY_SHIFT);
    }
}