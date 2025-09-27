#include "Engine.hpp"

#include <cstdlib>
#include <exception>
#include <csignal>
#include <iostream>

using namespace EngineRenderer;
using namespace EngineInput;
using namespace Engine;

GameEngine engine;

void handleCrash(int signal){
    std::cerr << "Caught Signal: " << signal << std::endl;
    engine.SaveScenes();
    engine.cleanup();
    std::_Exit(signal);
}

int main(){
    std::signal(SIGSEGV, handleCrash);
    std::signal(SIGABRT, handleCrash);

    try{
        engine.init();
        engine.mainLoop();
        engine.cleanup();
    } catch(const std::exception &e){
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        engine.SaveScenes();
        return EXIT_FAILURE;
    } catch(...){
        std::cerr << "Unkown Error Occured" << std::endl;
        engine.SaveScenes();
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}