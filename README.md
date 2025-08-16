
(All commits before August 15th were accidently deleted)

----------How to Build----------
1. Create a build and bin directory
    - mkdir build && mkdir bin
2. Navigate to build and build project
    - cd build
    - cmake ..
    - make
3. Navigate to bin and run the executable(./Vulkan-Renderer)
    - cd .. && cd bin
    - ./Vulkan-Renderer    

----------Required Libraries----------
1. Vulkan SDK(1.2+)
2. GLFW(3.3+)
3. stb(included in repo)

----------Notes----------
1. The only way to currently customize this renderer is through editing the source code
2. There is currently no way to contribute as this is a personal project
3. You may have to adjust CMake or linker settings depending on your platform(Linux/macOS/Windows)