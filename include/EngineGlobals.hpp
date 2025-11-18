#include "ECS/Components.hpp"
#include <vector>

//Reserved for things that get frequently used in differing systems that make no sense to encapsulate in a class or require and instance to access
namespace EngineGlobals{
    inline std::vector<Entity> changedBoundingBoxes;
}