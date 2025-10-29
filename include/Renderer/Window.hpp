#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace Engine{
    inline bool isFullscreen = false;

    void toggleFullscreen(GLFWwindow* window);
}

namespace EngineRenderer {
    class Window{
        public:
            void initWindow(int WIDTH, int HEIGHT, std::string name);

            void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR &surface);

            std::array<int, 2> getWindowSize();

            static void framebuffersrResizeCallback(GLFWwindow* window, int width, int height);

            GLFWwindow* getWindow(){
                return window;
            }

        private:
            GLFWwindow* window;
    };
}

#endif