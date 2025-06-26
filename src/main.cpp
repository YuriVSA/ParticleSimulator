#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>



float WIDTH = 800;
float HEIGHT = 600;
const int NUM_PARTICULAS = 100;

sf::VertexArray vertices(sf::Quads, NUM_PARTICULAS * 4);

std::random_device rd;
    std::mt19937 gen(rd());

//Distributions
std::uniform_real_distribution<float> distX(0.0f, WIDTH);
std::uniform_real_distribution<float> distY(0.0f, HEIGHT);
std::uniform_real_distribution<float> distVel(-50.0f, 50.0f);
std::uniform_int_distribution<int> distColor(0, 255);
std::uniform_real_distribution<float> distSize(2.0f, 8.0f);


struct Particle {
    sf::Vector2f position;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;

    float size;
    sf::Color color;
    float mass;

    Particle() : position(WIDTH / 2.0f, HEIGHT / 2.0f),
    acceleration(0.0f, 10.0f),
    velocity(0.0f, 0.0f),
    mass(1.0f),
    size(8.0f)
    {}
};


int main() {

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

        p.size = distSize(gen);
        p.color = sf::Color(distColor(gen), distColor(gen), distColor(gen));

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

        bool LeftClickON = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        bool RightClickON = sf::Mouse::isButtonPressed(sf::Mouse::Right);

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

            float size = p.size;
            

            if(p.position.x < size){
                p.position.x = size;
                p.velocity.x *= -0.9f;

            }
            else if(p.position.x > WIDTH - size){
                p.position.x = WIDTH - size;
                p.velocity.x *= -0.9f;
            }

            if(p.position.y < size){
                p.position.y = size;
                p.velocity.y *= -0.9f;
            }
            else if(p.position.y > HEIGHT - size){
                p.position.y = HEIGHT - size;
                p.velocity.y *= -0.9f;
            }
        }

        for (std::size_t i = 0; i < particles.size(); ++i) {
            const auto& p = particles[i];

            // Pega o ponteiro para o quad (os 4 vértices) desta partícula
            sf::Vertex* quad = &vertices[i * 4];

            // Define a posição dos 4 cantos do quadrado ao redor da posição da partícula
            quad[0].position = sf::Vector2f(p.position.x - p.size, p.position.y - p.size); // Canto sup esq
            quad[1].position = sf::Vector2f(p.position.x + p.size, p.position.y - p.size); // Canto sup dir
            quad[2].position = sf::Vector2f(p.position.x + p.size, p.position.y + p.size); // Canto inf dir
            quad[3].position = sf::Vector2f(p.position.x - p.size, p.position.y + p.size); // Canto inf esq

            // Define a cor para os 4 vértices
            quad[0].color = p.color;
            quad[1].color = p.color;
            quad[2].color = p.color;
            quad[3].color = p.color;
        }

        window.clear(sf::Color::White);

        //DRAW

        window.draw(vertices);
        window.draw(fpsText);

        
        window.display();
    }
    return 0;
}