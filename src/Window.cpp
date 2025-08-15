#include "Window.hpp"
#include "App.hpp"
#include <stdexcept>

namespace renderer {
    Window::Window(){
        initWindow(1000, 800, "Vulkan");
    }

    void Window::initWindow(int WIDTH, int HEIGHT, std::string name){
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(WIDTH, HEIGHT, name.c_str(), nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebuffersrResizeCallback);
    }

    void Window::createSurface(VkInstance instance, GLFWwindow *window, VkSurfaceKHR &surface){
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void Window::framebuffersrResizeCallback(GLFWwindow* window, int width, int height){
        if(width == 0 || height == 0) return;  

        auto app = reinterpret_cast<renderer::App*>(glfwGetWindowUserPointer(window));
        app->framebuffersrResized = true;
    }
}