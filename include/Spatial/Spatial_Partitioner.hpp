#ifndef _SPATIAL_PARTITIONER
#define _SPATIAL_PARTITIONER

#include "Object/Object.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

constexpr int CELL_SIZE = 10;

namespace EnginePartitioning{
    struct Cell{
        std::vector<EngineObject::Object*> objects;
    };

    class Spacial_Partitioner{
        private:
            std::unordered_map<uint64_t, Cell> grid;
        
        public:
            void registerObject(Object *object);

            std::vector<Cell*> getCells(Object *object);

            void reset() {grid.clear();}
    };
}

#endif