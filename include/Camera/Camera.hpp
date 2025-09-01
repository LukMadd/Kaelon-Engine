#include <glm/glm.hpp>
#include "Action.hpp"

using namespace EngineInput;

namespace EngineCamera{
    class Camera{
        public:
            Camera();
            glm::vec3 position;

        private:
            glm::vec3 front;
            glm::vec3 up;
            float speed = 10.0f;
            float velocity;
            float yaw = 0.0f;
            float pitch = 0.0f;
            float sensitivity = 0.1f;

            void moveUp(float velocity);
            void moveDown(float velocity);
            void moveForward(float velocity);
            void moveBackward(float velocity);
            void moveLeft(float velocity);
            void moveRight(float velocity);

        public:

            void updateCameraPosition(float deltaTime, ActionManager &actionManager);

            void updateYawAndPitch();

            glm::mat4 getViewMatrix();
    };
}