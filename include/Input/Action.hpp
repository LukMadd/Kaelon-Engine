#ifndef _ACTION_HPP
#define _ACTION_HPP

#include "Input.hpp"

namespace Engine{
    namespace Developer{
        enum DeveloperActions{
            CAMERA_FORWARD,
            CAMERA_BACKWARD,
            CAMERA_LEFT,
            CAMERA_RIGHT,
            CAMERA_UP,
            CAMERA_DOWN,
            EXIT_FULL_SCREEN,
            ENTER_FULL_SCREEN,
            SHOW_CURSOR,
            HIDE_CURSOR,
            CHANGE_SCENE_PLUS,
            CHANGE_SCENE_MINUS,
            TOGGLE_CAMERA_LOCK
        };
    }
    namespace Player{
        enum PlayerActions{
            MOVE_FORWARD,
            MOVE_BACKWARD,
            MOVE_LEFT,
            MOVE_RIGHT,
            Jump, //Not implemented
            Crouch, //Not implemented
            EXIT_FULLSCREEN,
            ENTER_FULLSCREEN,
        };
    }
}

using namespace Engine;

namespace EngineInput{
    class InputHandler;

    class ActionManager{
        public:
            void bind(int action, Key key);
            bool isActionActive(int action);

            void setupDeveloperBindings();

        private:
            std::unordered_map<int, Key> bindedActions;
            InputHandler *inputHandler;
    };
}

#endif
