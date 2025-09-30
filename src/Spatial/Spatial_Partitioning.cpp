#include "Spatial/Spatial_Partitioner.hpp"
#include <cmath>
#include <cstdint>

constexpr int MIN_CELL_X_INDEX = 0;
constexpr int MIN_CELL_Y_INDEX = 1;
constexpr int MIN_CELL_Z_INDEX = 2;
constexpr int MAX_CELL_X_INDEX = 3;
constexpr int MAX_CELL_Y_INDEX = 4;
constexpr int MAX_CELL_Z_INDEX = 5;


namespace EnginePartitioning{
    uint64_t hash(int cellX, int cellY, int cellZ){
        uint64_t key = uint64_t(cellX) * 73856093 ^ uint64_t(cellY) * 19349663 ^ uint64_t(cellZ) * 83492791;
        return key;
    }

    std::array<int, 6> getObjectCellCoordinates(Object *object){
        glm::vec3 worldMin = object->worldBoundingBox.min;
        glm::vec3 worldMax = object->worldBoundingBox.max;

        int minCellX = floor(worldMin.x / CELL_SIZE);
        int minCellY = floor(worldMin.y / CELL_SIZE);
        int minCellZ = floor(worldMin.z / CELL_SIZE);

        int maxCellX = floor(worldMax.x / CELL_SIZE);
        int maxCellY = floor(worldMax.y / CELL_SIZE);
        int maxCellZ = floor(worldMax.z / CELL_SIZE);

        std::array<int, 6> coords;
        coords[MIN_CELL_X_INDEX] = minCellX;
        coords[MIN_CELL_Y_INDEX] = minCellY;
        coords[MIN_CELL_Z_INDEX] = minCellZ;
        coords[MAX_CELL_X_INDEX] = maxCellX;
        coords[MAX_CELL_Y_INDEX] = maxCellY;
        coords[MAX_CELL_Z_INDEX] = maxCellZ;

        return coords;
    }

    void Spacial_Partitioner::registerObject(Object *object, std::vector<uint64_t> &cellKeys){
        for(auto &cellKey : cellKeys){
            grid[cellKey].objects.reserve(10); //Reserves space for 10 objects in a cell
            grid[cellKey].objects.insert(object);

            object->cells.push_back(cellKey);
        }

        object->assignedToCell = true;
    }

    std::vector<uint64_t> Spacial_Partitioner::getCellKeys(Object *object){
        std::vector<uint64_t> cellKeys;

        std::array<int, 6> objectCellCoords = getObjectCellCoordinates(object);

        for(int x = objectCellCoords[MIN_CELL_X_INDEX]; x <= objectCellCoords[MAX_CELL_X_INDEX]; x++){
            for(int y = objectCellCoords[MIN_CELL_Y_INDEX]; y <= objectCellCoords[MAX_CELL_Y_INDEX]; y++){
                for(int z = objectCellCoords[MIN_CELL_Z_INDEX]; z <= objectCellCoords[MAX_CELL_Z_INDEX]; z++){
                    cellKeys.push_back(hash(x, y, z));
                }
            }
        }

        return cellKeys;
    }

    void Spacial_Partitioner::removeObject(Object *object){
        std::vector<uint64_t> cellsToRemove;

        for(auto key : object->cells){
            grid[key].objects.erase(object);
            cellsToRemove.push_back(key);
        }

        object->cells.clear();
    }

    void Spacial_Partitioner::reRegisterObject(Object *object, std::vector<uint64_t> &cellKeys){
        removeObject(object);

        registerObject(object, cellKeys);
    }



    std::vector<Cell*> Spacial_Partitioner::getCells(Object *object){
        std::vector<Cell*> cells;

        for(int i = 0; i < object->cells.size(); i++){
            cells.push_back(&grid[object->cells[i]]);
        }

        return cells;
    }
}