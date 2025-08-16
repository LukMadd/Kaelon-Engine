#-----------------------------------------General Setup-----------------------------------------#
include(FetchContent)

cmake_minimum_required(VERSION 3.28.0)

project(Vulkan-Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#-----------------------------------------Find GLFW and Vulkan-----------------------------------------#
FetchContent_Declare(
    stb
    URL https://github.com/nothings/stb/archive/refs/heads/master.zip
)

FetchContent_MakeAvailable(stb)

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG release
)

FetchContent_MakeAvailable(tinyobjloader)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

if(DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
  set(Vulkan_LIB_DIR "${VULKAN_SDK_PATH}/lib")
  set(Vulkan_FOUND TRUE)
else()
  find_package(Vulkan REQUIRED)
  set(Vulkan_INCLUDE_DIRS "$ENV{VULKAN_SDK}/Include")
  set(Vulkan_LIB_DIR "$ENV{VULKAN_SDK}/lib")
  set(Vulkan_FOUND TRUE)
  message(STATUS "Found Vulkan SDK at: $ENV{VULKAN_SDK}")
endif()

if(NOT Vulkan_FOUND)
  message(FATAL_ERROR "Could not find Vulkan library!")
else()
  message(STATUS "Using vulkan include dir: ${Vulkan_INCLUDE_DIRS}")
  message(STATUS "Using vulkan lib dir: ${Vulkan_LIB_DIR}")
endif()

if(DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env: ${GLFW_PATH}")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  set(GLFW_LIB_DIR "${GLFW_PATH}/lib")

  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2019")

  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64")
  endif()

else()
  find_package(glfw3 3.3 REQUIRED)
  set(GLFW_INCLUDE_DIRS "${glfw3_DIR}/include")
  set(GLFW_LIB_DIR "${glfw3_DIR}/lib")
  message(STATUS "Found GLFW: ${GLFW_INCLUDE_DIRS}")
endif()

if(NOT GLFW_LIB_DIR)
  message(FATAL_ERROR "Could not find glfw library!")
else()
  message(STATUS "Using glfw include dir: ${GLFW_INCLUDE_DIRS}")
  message(STATUS "Using glfw lib dir: ${GLFW_LIB_DIR}")
endif()

find_package(glm REQUIRED)

#-----------------------------------------Set source files-----------------------------------------#
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/Renderer/*.cpp")

add_executable(Vulkan-Renderer ${SOURCES})

#-----------------------------------------Find glslang-----------------------------------------#
find_program(GLSLANG_VALIDATOR glslangValidator)

if(NOT GLSLANG_VALIDATOR)
    message(FATAL_ERROR "Please install glslangValidator(brew install glslang)")
else()
    message(STATUS "Glslang validator found: ${GLSLANG_VALIDATOR}")
endif()

#-----------------------------------------Shader compilation-----------------------------------------#
file(GLOB SHADER_SOURCES "${CMAKE_SOURCE_DIR}/Renderer/shaders/*.vert" "${CMAKE_SOURCE_DIR}/Renderer/shaders/*.frag")

set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

set(SPIRV_SHADERS "")

foreach(SHADER_FILE ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
    get_filename_component(SHADER_EXT ${SHADER_FILE} EXT)
    string(SUBSTRING ${SHADER_EXT} 1 -1 SHADER_EXT_NO_DOT)
    set(SHADER_OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_NAME}_${SHADER_EXT_NO_DOT}.spv")

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER_FILE} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER_FILE}
        VERBATIM
    )

    list(APPEND SPIRV_SHADERS ${SHADER_OUTPUT})
endforeach()

add_custom_target(
    Shaders ALL
    DEPENDS ${SPIRV_SHADERS}
)

set(RUNTIME_SHADER_DIR "${CMAKE_SOURCE_DIR}/bin/shaders")
file(MAKE_DIRECTORY ${RUNTIME_SHADER_DIR})

add_custom_command(
    TARGET Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_OUTPUT_DIR} ${RUNTIME_SHADER_DIR}
)

add_dependencies(Vulkan-Renderer Shaders)

#-----------------------------------------Include directories and libraries-----------------------------------------#
target_include_directories(Vulkan-Renderer PRIVATE
    ${CMAKE_SOURCE_DIR}/include/Renderer
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${tinyobjloader_SOURCE_DIR}
)

message(STATUS "GLFW LIB PATH: ${GLFW_LIB_DIR}")
target_link_libraries(Vulkan-Renderer PRIVATE
    glfw
    glm::glm
    "${Vulkan_LIB_DIR}/libvulkan.dylib"
    stb_image
)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

target_link_libraries(Vulkan-Renderer PRIVATE
    "${Vulkan_LIB_DIR}/libMoltenVK.dylib"
    "-framework QuartzCore"
    "-framework CoreFoundation"
    "-framework CoreGraphics"
    "-framework IOKit"
    "-framework IOSurface"
    "-framework Metal"
    "-framework MetalKit"
    "-framework Foundation"
    "-framework AppKit"
)
endif()
