#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <Particle.hpp>
#include <spacialGrid.hpp>



float WIDTH = 800;
float HEIGHT = 600;
float CellSize = 6.0f;
const int NUM_PARTICULAS = 10000;
const int SOLVER_ITERATIONS = 1;

sf::VertexArray vertices(sf::Quads, NUM_PARTICULAS * 4);

std::random_device rd;
    std::mt19937 gen(rd());

//Distributions
std::uniform_real_distribution<float> distX(0.0f, WIDTH);
std::uniform_real_distribution<float> distY(0.0f, HEIGHT);
std::uniform_real_distribution<float> distVel(-50.0f, 50.0f);
std::uniform_int_distribution<int> distColor(0, 255);
std::uniform_real_distribution<float> distSize(2.0f, 8.0f);
std::uniform_real_distribution<float> distMass(1.0f, 10.0f);



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

    sf::Text ParticulasText;
    ParticulasText.setFont(font);
    ParticulasText.setCharacterSize(20);
    ParticulasText.setFillColor(sf::Color::Black);
    ParticulasText.setString("Num Particulas: " + std::to_string(NUM_PARTICULAS));
    float textWidth = ParticulasText.getGlobalBounds().width;
    ParticulasText.setPosition(WIDTH - textWidth - 10.0f, 10.0f);
    

    std::vector<Particle> particles;
    particles.reserve(NUM_PARTICULAS);

    for(int i = 0; i < NUM_PARTICULAS; i++){
        Particle p(WIDTH, HEIGHT);

        p.position = sf::Vector2f(distX(gen), distY(gen));
        p.velocity = sf::Vector2f(distVel(gen), distVel(gen));

        //p.mass = distMass(gen);
        //p.color = sf::Color(distColor(gen), distColor(gen), distColor(gen));
        //p.size = distSize(gen);
        p.color = sf::Color::Blue;
        

        particles.push_back(p);
    }

    
    SpacialGrid grid(WIDTH, HEIGHT, CellSize);

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

            if (ev.type == sf::Event::KeyPressed) {
            
                if (ev.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        //UPDATE

        for (auto& p : particles) {

            sf::Vector2f totalForce(0.0f, 0.0f); 
            sf::Vector2f gravityForce(0.0f, 98.1f * p.mass);

            totalForce += gravityForce;

            if (p.mass > 0.0f) { // Evita divisão por zero
                 p.acceleration = totalForce / p.mass;
            }
            p.velocity += p.acceleration * dt;
            p.position += p.velocity * dt;
        }
        
        grid.clear();
        for (auto& p : particles) {
            grid.add(&p);
        }

        // Collision Detection
        for (int i = 0; i < SOLVER_ITERATIONS; ++i) {
            for(auto& p1 : particles){
                std::vector<Particle*> neighbors = grid.getNeighbors(&p1);

                for(Particle* p2 : neighbors){
                    if(&p1 == p2) continue;
                    if (p2 < &p1) continue;

                    sf::Vector2f collisionAxis = p1.position - p2->position;
                    float dist2 = collisionAxis.x * collisionAxis.x + collisionAxis.y * collisionAxis.y;
                    float combinedRadius = p1.size + p2->size;

                    if (dist2 < combinedRadius * combinedRadius && dist2 > 0.0001f) {
                        float distance = std::sqrt(dist2);

                        
                        sf::Vector2f normal = collisionAxis / distance;

                        //Overlap
                        float overlap = 0.5f * (combinedRadius - distance);

                        p1.position += normal * overlap;
                        p2->position -= normal * overlap;

                        //Resolution
                        sf::Vector2f tangent(-normal.y, normal.x);
                        float dpTan1 = p1.velocity.x * tangent.x + p1.velocity.y * tangent.y;
                        float dpTan2 = p2->velocity.x * tangent.x + p2->velocity.y * tangent.y;
                        float dpNorm1 = p1.velocity.x * normal.x + p1.velocity.y * normal.y;
                        float dpNorm2 = p2->velocity.x * normal.x + p2->velocity.y * normal.y;
                        float m1 = (dpNorm1 * (p1.mass - p2->mass) + 2.0f * p2->mass * dpNorm2) / (p1.mass + p2->mass);
                        float m2 = (dpNorm2 * (p2->mass - p1.mass) + 2.0f * p1.mass * dpNorm1) / (p1.mass + p2->mass);
                        p1.velocity = tangent * dpTan1 + normal * m1;
                        p2->velocity = tangent * dpTan2 + normal * m2;
                    }
                }
            }
        }    
        
        // Collision Walls and Floor
        for(auto& p : particles){
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
        window.draw(ParticulasText);

        
        window.display();
    }
    return 0;
}