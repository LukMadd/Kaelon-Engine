
(All commits before August 15th were accidently deleted)

----------How to Build----------
1. Create a build and bin directory
    - mkdir build
2. Navigate to build and build project
    - cd build
    - cmake ..
    - make ("make -j" recommended for speed)
3. Navigate to bin and run the executable(./Kaelon-Engine)
    - cd .. && cd bin
    - ./Kaelon-Engine   

----------Required Libraries----------
1. Vulkan SDK(1.2+)
2. GLFW(3.3+)
3. GLM
4. ImGui(included in repo)
5. stb(included in repo)
6. tinyobjLoader(included in repo)

----------Notes----------
1. The only way to currently customize this renderer is through editing the source code
2. There is currently no way to contribute as this is a personal project
3. You may have to adjust CMake or linker settings depending on your platform(Linux/macOS/Windows)

----------Current Commands----------
1. WASD for movement
2. Space and tab for up and down
3. F10 for full screen
4. Esc for exiting fullscreen
3. '~' for showing cursor
4. Shift for hiding cursor
6. "." to lock camera


(These are all editable in Input.hpp(for linking custom keys to GLFW keys), Action.hpp(For creating unique actions), and Action.cpp(for linking keys to the actions))