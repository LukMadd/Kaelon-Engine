#include "Object/UUID.hpp"

#include <random>
#include <sstream>

namespace EngineObject{
    std::string generateUUID(){
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);

        std::stringstream ss;
        for(int i = 0; i < 32; ++i){
            ss << std::hex << dis(gen);
        }
        return ss.str();
    }
}