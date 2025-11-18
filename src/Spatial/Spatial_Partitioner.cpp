#include "ECS/Components.hpp"
#include "ECS/EntityFunctions.hpp"
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

    std::array<int, 6> getObjectCellCoordinates(BoundingBoxComponent* boundingBox){
        glm::vec3 worldMin = boundingBox->worldBoundingBox.min;
        glm::vec3 worldMax = boundingBox->worldBoundingBox.max;

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

    void Spatial_Partitioner::addEntity(Entity e){
        std::vector<uint64_t> cellKeys = getCellKeys(e);

        registerEntity(e, cellKeys);
    }

    void Spatial_Partitioner::registerEntity(Entity entity, std::vector<uint64_t> &cellKeys){
        auto* spatial = ecs->getComponent<SpatialPartitioningComponent>(entity);

        for(auto &cellKey : cellKeys){
            grid[cellKey].entities.reserve(10); //Reserves space for 10 objects in a cell
            grid[cellKey].entities.insert(entity);

            spatial->cells.push_back(cellKey);
        }

        spatial->assignedToCell = true;
    }

    std::vector<uint64_t> Spatial_Partitioner::getCellKeys(Entity entity){
        auto* boundingBox = ecs->getComponent<BoundingBoxComponent>(entity);
        if(!boundingBox->localBoundingBox.isInitialized || !boundingBox->worldBoundingBox.isInitialized){
            createBoundingBox(entity, *ecs);
        }
        std::vector<uint64_t> cellKeys;

        std::array<int, 6> objectCellCoords = getObjectCellCoordinates(boundingBox);

        for(int x = objectCellCoords[MIN_CELL_X_INDEX]; x <= objectCellCoords[MAX_CELL_X_INDEX]; x++){
            for(int y = objectCellCoords[MIN_CELL_Y_INDEX]; y <= objectCellCoords[MAX_CELL_Y_INDEX]; y++){
                for(int z = objectCellCoords[MIN_CELL_Z_INDEX]; z <= objectCellCoords[MAX_CELL_Z_INDEX]; z++){
                    cellKeys.push_back(hash(x, y, z));
                }
            }
        }

        return cellKeys;
    }

    void Spatial_Partitioner::updateEntityCells(Entity entity){
        std::vector<uint64_t> cellKeys = getCellKeys(entity);
        auto* spatial = ecs->getComponent<SpatialPartitioningComponent>(entity);

        if(cellKeys != spatial->cells){
            if(spatial->assignedToCell == false){
                registerEntity(entity, cellKeys);
            } else{
                reRegisterEntity(entity, cellKeys);
            }
        }
    }

    void Spatial_Partitioner::removeEntity(Entity entity){
        std::vector<uint64_t> cellsToRemove;

        auto* spatial = ecs->getComponent<SpatialPartitioningComponent>(entity);

        for(auto key : spatial->cells){
            grid[key].entities.erase(entity);
            cellsToRemove.push_back(key);
        }

        spatial->cells.clear();
    }

    void Spatial_Partitioner::reRegisterEntity(Entity entity, std::vector<uint64_t> &cellKeys){
        removeEntity(entity);

        registerEntity(entity, cellKeys);
    }


    std::vector<Cell*> Spatial_Partitioner::getCells(Entity entity){
        std::vector<Cell*> cells;

        auto* spatial = ecs->getComponent<SpatialPartitioningComponent>(entity);

        for(int i = 0; i < spatial->cells.size(); i++){
            cells.push_back(&grid[spatial->cells[i]]);
        }

        return cells;
    }
}