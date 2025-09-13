#ifndef _INPUT_HANDLER_HPP
#define _INPUT_HANDLER_HPP

#include "Input.hpp"
#include "Action.hpp"
#include "SceneManager.hpp"

namespace EngineInput{
    class InputHandler{
        public:
            bool isPressed(Key key);

            void update(GLFWwindow* window, 
                        ActionManager &actionManager, 
                        EngineScene::SceneManager &sceneManager);

            bool isSceneImmersed(){return is_scene_immersed;}

        private:
            bool is_scene_immersed = true;
            bool was_immersed_action_active = false; //Used to help with preventing multiple scene immersion flips in one frame
    };
}

#endif