#include "Engine.hpp"

#include <cstdlib>

using namespace EngineRenderer;
using namespace EngineInput;
using namespace Engine;

int main(){
    GameEngine engine;

    engine.init();
    engine.mainLoop();
    engine.cleanup();

    return EXIT_SUCCESS;
}