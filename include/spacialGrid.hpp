#ifndef SPACIALGRID_HPP
#define SPACIALGRID_HPP
#include <iostream>
#include <vector>
#include <unordered_map>
#include <Particle.hpp>


struct Particle;

class SpacialGrid
{
private:
    std::unordered_map<int, std::vector<Particle*>> m_grid;
    float cellSize;
    int numCollumns;

    int getKey(const sf::Vector2f& position);
    int getKey(int cellX, int cellY);

public:
    SpacialGrid(float widht, float height, float cellSize);
    ~SpacialGrid();


    void add(Particle *p);
    std::vector<Particle*> getNeighbors(Particle *p);

    void clear();
};












#endif