#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "src/TmxLoaderPrueba.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "src/boy.hpp"

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mapa de Tiled con SFML");
    window.setFramerateLimit(60);  // Mejorar consistencia de FPS

    // ===== 1. Carga de recursos =====
    sf::Texture tileset;
    if (!tileset.loadFromFile("Textures/cave_tileset.png")) {
        std::cerr << "Error: Tileset no encontrado." << std::endl;
        return -1;
    }

    std::vector<tmx::TileLayer> layers;
    if (!tmx::TMXLoader::Load("Textures/LevelDesignTest.tmx", layers)) {
        std::cerr << "Error: Mapa TMX no cargado." << std::endl;
        return -1;
    }

    // ===== 2. Configuración del mapa =====
    const sf::Vector2u tileSize(32, 32);
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    bool firstChunk = true;

    for (const auto& layer : layers) {
        for (const auto& chunk : layer.chunks) {
            // Asumir que chunks usan coordenadas en tiles
            int chunkRight = chunk.x + chunk.width;
            int chunkBottom = chunk.y + chunk.height;

            if (firstChunk) {
                minX = chunk.x;
                maxX = chunkRight;
                minY = chunk.y;
                maxY = chunkBottom;
                firstChunk = false;
            } else {
                minX = std::min(minX, chunk.x);
                maxX = std::max(maxX, chunkRight);
                minY = std::min(minY, chunk.y);
                maxY = std::max(maxY, chunkBottom);
            }
        }
    }

    // Calcular dimensiones reales en píxeles
    int mapWidth = (maxX - minX) * tileSize.x;
    int mapHeight = (maxY - minY) * tileSize.y;

    // ===== 3. Pantalla virtual para el mapa =====
    sf::RenderTexture virtualScreen;
    if (!virtualScreen.create(mapWidth, mapHeight)) {
        std::cerr << "Error: RenderTexture no creada." << std::endl;
        return -1;
    }
    virtualScreen.clear(sf::Color::Transparent);

    // Dibujar todas las capas
    for (const auto& layer : layers) {
        tmx::TMXLoader::DrawTileMap(virtualScreen, tileset, layer, tileSize, 
                                  -minX * tileSize.x, -minY * tileSize.y);
    }
    virtualScreen.display();

    // ===== 4. Configuración del personaje =====
    Boy boy;
    if (!boy.loadTexture("Textures/boyWalk.png")) {  // Ruta corregida
        std::cerr << "Error: Textura del personaje no cargada." << std::endl;
        return -1;
    }
    boy.setPosition(mapWidth/2.0f, mapHeight/2.0f);  // Centro del mapa

    // ===== 5. Configuración de la cámara =====
    sf::View camera = window.getDefaultView();
    camera.setSize(800, 600);
    camera.setCenter(boy.getPosition());

    sf::Clock deltaClock;

    // ===== 6. Bucle principal =====
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            // Zoom con rueda del ratón
            if (event.type == sf::Event::MouseWheelScrolled) {
                float zoom = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                camera.zoom(zoom);
            }
        }

        // ===== Actualizar lógica =====
        float deltaTime = deltaClock.restart().asSeconds();
        
        // Mover personaje con WASD
        float speed = 200.0f * deltaTime;
        bool anyKeyPressed = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            boy.move(0, -speed);
            boy.setAnimationRow(1);
            anyKeyPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            boy.move(0, speed);
            boy.setAnimationRow(0);
            anyKeyPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            boy.move(-speed, 0);
            boy.setAnimationRow(3);
            anyKeyPressed = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            boy.move(speed, 0);
            boy.setAnimationRow(2);
            anyKeyPressed = true;
        }
        if (!anyKeyPressed) {
        boy.stopMoving();        
        //boy.setAnimationRow(boy.getLastMovementRow());
        }
        boy.update(deltaTime);  // Actualizar animación
        camera.setCenter(boy.getPosition());  // Cámara sigue al personaje

        // ===== Renderizado =====
        window.clear(sf::Color::Black);
        window.setView(camera);

        // Dibujar mapa
        sf::Sprite mapSprite(virtualScreen.getTexture());
        window.draw(mapSprite);

        // Dibujar personaje
        boy.draw(window);

        window.display();
    }

    return 0;
}