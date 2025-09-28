#include "UI/FPSManager.hpp"

namespace EngineUI{
    //Smooths FPS counter so that it is readable
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

    float FPSManager::smoothFPS(float fps){
        if(smoothedFPS <= 0.0f){
            smoothedFPS = fps;
        }else {
            smoothedFPS = alpha * fps + (1.0f - alpha) * smoothedFPS;
        }
        return smoothedFPS;
    }
}