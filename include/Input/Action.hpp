#ifndef _ACTION_HPP
#define _ACTION_HPP

#include "Input.hpp"

namespace Engine{
    enum class Action{
        PLAYER_MOVE_UP,
        PLAYER_MOVE_DOWN,
        PLAYER_MOVE_FORWARD,
        PLAYER_MOVE_BACKWARD,
        PLAYER_MOVE_LEFT,
        PLAYER_MOVE_RIGHT,
        PLAYER_JUMP,
        PLAYER_CROUCH,
        USER_ESCAPE_PROGRAM,
        USER_FULL_SCREEN,
        USER_HIDE_CURSOR
    };
}

using namespace Engine;

namespace EngineInput{
    class InputHandler;

    class ActionManager{
        public:
            void bind(Action action, Key key);
            bool isActionActive(Action action);

            void setupBindings();

        private:
            std::unordered_map<Action, Key> bindedActions;
            InputHandler *inputHandler;
    };
}

#endif