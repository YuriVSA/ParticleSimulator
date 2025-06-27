#include "spacialGrid.hpp"
#include "cmath"


SpacialGrid::SpacialGrid(float width, float height, float cellSize) : cellSize(cellSize),
    numCollumns(static_cast<int>(std::ceil(width / cellSize))) {}
    
SpacialGrid::~SpacialGrid() {}

int SpacialGrid::getKey(const sf::Vector2f& position){
    int cellX = static_cast<int>(position.x / cellSize);
    int cellY = static_cast<int>(position.y / cellSize);

    return cellX + cellY * numCollumns;
}

int SpacialGrid::getKey(int cellX, int cellY) {
    return cellX + cellY * numCollumns;
}


void SpacialGrid::clear(){
    m_grid.clear();
}

void SpacialGrid::add(Particle* p) {
    
    int key = getKey(p->position);

    m_grid[key].push_back(p);
}


std::vector<Particle*> SpacialGrid::getNeighbors(Particle *p) {
    std::vector<Particle*> neighbors;

    int cellX = static_cast<int>(p->position.x / cellSize);
    int cellY = static_cast<int>(p->position.y / cellSize);

    for(int x = cellX - 1; x <= cellX + 1; ++x){
        for(int y = cellY -1; y <= cellY + 1; ++y){
            int key = getKey(x, y);
            if (m_grid.count(key)) {
                auto& cellParticles = m_grid.at(key);
                neighbors.insert(neighbors.end(), cellParticles.begin(), cellParticles.end());
            }
        }
    }

    return neighbors;
}