#include "Window.hpp"
#include "Renderer.hpp"
#include <stdexcept>
#include <glm/glm.hpp>

#include "Input.hpp"

namespace Engine{
    void toggleFullscreen(GLFWwindow* window){
        static int windowedX, windowedY, windowedWidth, windowedHeight;

        if(!isFullscreen){
            glfwGetWindowPos(window, &windowedX, &windowedY);
            glfwGetWindowSize(window, &windowedWidth, & windowedHeight);

            GLFWmonitor *primary = glfwGetPrimaryMonitor();
            const GLFWvidmode *videoMode = glfwGetVideoMode(primary);

            glfwSetWindowMonitor(window, primary, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
        } else{
            glfwSetWindowMonitor(window, nullptr, windowedX, windowedY, windowedWidth, windowedHeight, 0);
        }

        EngineInput::Input::get().resetMouse();
        isFullscreen = !isFullscreen;
    }
}

namespace EngineRenderer {
    Window::Window(){
        initWindow(1000, 800, "Vulkan");
    }

    void Window::initWindow(int WIDTH, int HEIGHT, std::string name){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, name.c_str(), nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebuffersrResizeCallback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSwapInterval(0);
    }

    void Window::createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR &surface){
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void Window::framebuffersrResizeCallback(GLFWwindow* window, int width, int height){
        if(width == 0 || height == 0) return;  

        auto app = reinterpret_cast<EngineRenderer::Renderer*>(glfwGetWindowUserPointer(window));
        app->framebuffersrResized = true;
    }
}