#ifndef PARTICLE_HEADER_HPP
#define PARTICLE_HEADER_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>


struct Particle {
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;

    
    sf::Color color;
    float mass;
    float size;

    Particle(float WIDTH, float HEIGHT) : position(WIDTH / 2.0f, HEIGHT / 2.0f),
    acceleration(0.0f, 10.0f),
    velocity(0.0f, 0.0f),
    mass(1.0f),
    size(3.0f)
    {}
};

#endif