#ifndef ENGINE_CONTEXT_HPP
#define ENGINE_CONTEXT_HPP

#include "Scene/SceneManager.hpp"
#include "ECS/ECS.hpp"

using namespace EngineScene;

struct EngineContext{
  EngineContext() : sceneManager(){};

  EngineContext& operator=(EngineContext& other){
    if(this == &other){
      return *this;
    }

    sceneManager = std::move(other.sceneManager);

    ecs.setComponentStorage(other.ecs.getComponentStorage());

    return *this;
  }

  SceneManager sceneManager;
  ECS ecs;

  bool isSetup = false;
};

#endif
