#ifndef _SPATIAL_PARTITIONER
#define _SPATIAL_PARTITIONER

#include "Object/Object.hpp"
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

constexpr int CELL_SIZE = 10;

namespace EnginePartitioning{
    struct Cell{
        std::unordered_set<EngineObject::Object*> objects;
    };

    class Spacial_Partitioner{
        private:
            std::unordered_map<uint64_t, Cell> grid;

            void removeObject(Object *object);
        
        public:
            void registerObject(Object *object, std::vector<uint64_t> &cellKeys);

            std::vector<uint64_t> getCellKeys(Object *object);

            std::vector<Cell*> getCells(Object *object);

            void reRegisterObject(Object *object, std::vector<uint64_t> &cellKeys);

            void reset() {grid.clear();}
    };
}

#endif