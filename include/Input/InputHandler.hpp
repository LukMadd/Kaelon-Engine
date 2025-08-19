#ifndef _INPUT_HANDLER_HPP
#define _INPUT_HANDLER_HPP

#include "Input.hpp"
#include "Action.hpp"

namespace EngineInput{
    class InputHandler{
        public:
            bool isPressed(Key key);

            void update(GLFWwindow* window, ActionManager &actionManager);
    };
}

#endif