#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>



float WIDTH = 800;
float HEIGHT = 600;
const int NUM_PARTICULAS = 5000;

struct Particle {
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;


    float mass;
    

    sf::CircleShape shape;

    Particle() : position(WIDTH / 2.0f, HEIGHT / 2.0f),
    acceleration(0.0f, 10.0f),
    velocity(0.0f, 0.0f),
    mass(1.0f)
    {}
};


int main() {

    std::random_device rd;
    std::mt19937 gen(rd());

    //Distributions
    std::uniform_real_distribution<float> distX(0.0f, WIDTH);
    std::uniform_real_distribution<float> distY(0.0f, HEIGHT);
    std::uniform_real_distribution<float> distVel(-50.0f, 50.0f);
    std::uniform_int_distribution<int> distColor(0, 255);
    std::uniform_real_distribution<float> distSize(2.0f, 8.0f);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Janela SFML");

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
        return -1; // Aborta se não encontrar a fonte
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24); // Em pixels
    fpsText.setFillColor(sf::Color::Black);
    fpsText.setPosition(10.0f, 10.0f);

    std::vector<Particle> particles;
    particles.reserve(NUM_PARTICULAS);

    for(int i = 0; i < NUM_PARTICULAS; i++){
        Particle p;

        p.position = sf::Vector2f(distX(gen), distY(gen));
        p.velocity = sf::Vector2f(distVel(gen), distVel(gen));
        
        float randomSize = distSize(gen);

        p.shape.setRadius(randomSize);
        p.shape.setOrigin(randomSize, randomSize);
        p.shape.setFillColor(sf::Color(distColor(gen), distColor(gen),distColor(gen)));


        particles.push_back(p);
    }

    

    window.setFramerateLimit(60);

    sf::Clock clock;

    //OPEN WINDOW
    while(window.isOpen()){

        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds(); // Delta time em segundos

        float fps = 1.0f / dt;
        fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

        sf::Event ev;

        while(window.pollEvent(ev)){

            if(ev.type == sf::Event::Closed){
                window.close();
            }
        }

        //UPDATE
        for (auto& p : particles) {
            sf::Vector2f totalforce = p.acceleration * p.mass;
            
            p.velocity += p.acceleration * dt;
            p.position += p.velocity * dt;

            if(p.position.x < p.shape.getRadius()){
                p.position.x = p.shape.getRadius();
                p.velocity *= -0.9f;

            }
            else if(p.position.x > WIDTH - p.shape.getRadius()){
                p.position.x = WIDTH - p.shape.getRadius();
                p.velocity *= 0.9f;
            }

            if(p.position.y < p.shape.getRadius()){
                p.position.y = p.shape.getRadius();
                p.velocity *= -0.9f;
            }
            else if(p.position.y > HEIGHT - p.shape.getRadius()){
                p.position.y = HEIGHT - p.shape.getRadius();
                p.velocity *= -0.9f;
            }
        }

        window.clear(sf::Color::White);

        //DRAW
        for (auto& p : particles) { 
            p.shape.setPosition(p.position);
            window.draw(p.shape);
        }

        window.draw(fpsText);

        
        window.display();
    }
    return 0;
}