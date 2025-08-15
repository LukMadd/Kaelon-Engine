#ifndef _WINDOW_HPP
#define _WINDOW_HPP


#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


namespace renderer {
    class Window{
        public:
            Window();

            void initWindow(int WIDTH, int HEIGHT, std::string name);

            void createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR &surface);

            static void framebuffersrResizeCallback(GLFWwindow* window, int width, int height);

            GLFWwindow* getWindow(){
                return window;
            }

        private:
            GLFWwindow* window;
    };
}

#endif