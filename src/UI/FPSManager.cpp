#include "FPSManager.hpp"

namespace EngineUI{
    float FPSManager::updateFPS(float deltaTime){
        frameTimeSum += deltaTime;
        frameCount++;

        if(frameCount > smoothingWindow){
            fps = frameCount / frameTimeSum;
            frameTimeSum = 0.0f;
            frameCount = 0;
        }

        return fps;
    }
}