#ifndef _INPUT_HANDLER_HPP
#define _INPUT_HANDLER_HPP

#include "Input.hpp"
#include "Action.hpp"
#include "Scene/SceneManager.hpp"

namespace EngineInput{
    class InputHandler{
        public:
            bool isPressed(Key key);

            void update(GLFWwindow* window, 
                        ActionManager &actionManager, 
                        EngineScene::SceneManager &sceneManager);

            void setBindings(const std::unordered_map<int, std::function<void()>>& bindings){
                inputBindings = bindings;
            }

        private:
            std::unordered_map<int, std::function<void()>> inputBindings;
    };
}

#endif