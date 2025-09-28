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

    void Spacial_Partitioner::registerObject(Object *object){
        std::array<int, 6> objectCellCoords = getObjectCellCoordinates(object);

        for(int x = objectCellCoords[MIN_CELL_X_INDEX]; x <= objectCellCoords[MAX_CELL_X_INDEX]; x++){
            for(int y = objectCellCoords[MIN_CELL_Y_INDEX]; y <= objectCellCoords[MAX_CELL_Y_INDEX]; y++){
                for(int z = objectCellCoords[MIN_CELL_Z_INDEX]; z <= objectCellCoords[MAX_CELL_Z_INDEX]; z++){
                    uint64_t key = hash(x, y, z);
                    grid[key].objects.reserve(10);
                    grid[key].objects.push_back(object);
                }
            }
        }
    }

    std::vector<Cell*> Spacial_Partitioner::getCells(Object *object){
        std::array<int, 6> objectCellCoords = getObjectCellCoordinates(object);

        std::vector<Cell*> cells;

        for(int x = objectCellCoords[MIN_CELL_X_INDEX]; x <= objectCellCoords[MAX_CELL_X_INDEX]; x++){
            for(int y = objectCellCoords[MIN_CELL_Y_INDEX]; y <= objectCellCoords[MAX_CELL_Y_INDEX]; y++){
                for(int z = objectCellCoords[MIN_CELL_Z_INDEX]; z <= objectCellCoords[MAX_CELL_Z_INDEX]; z++){
                    uint64_t key = hash(x, y, z);
                    cells.push_back(&grid[key]);
                }
            }
        }

        return cells;
    }
}