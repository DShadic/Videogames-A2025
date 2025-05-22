#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "src/TmxLoaderPrueba.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

int main() 
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mapa de Tiled con SFML");

    // 1. Cargar recursos
    sf::Texture tileset;
    if (!tileset.loadFromFile("Textures/cave_tileset.png")) {
        std::cerr << "Error al cargar el tileset." << std::endl;
        return -1;
    }

    std::vector<tmx::TileLayer> layers;
    if (!tmx::TMXLoader::Load("Textures/LevelDesignTest.tmx", layers)) {
        std::cerr << "Error al cargar el mapa." << std::endl;
        return -1;
    }

    const sf::Vector2u tileSize(32, 32);

    // 2. Calcular dimensiones totales del mapa
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    bool firstChunk = true;

    for (const auto& layer : layers) 
    {
        for (const auto& chunk : layer.chunks) 
        {
            int chunkLeft = chunk.x;
            int chunkRight = chunk.x + chunk.width;
            int chunkTop = chunk.y;
            int chunkBottom = chunk.y + chunk.height;
            
            if (firstChunk) 
            {
                minX = chunkLeft;
                maxX = chunkRight;
                minY = chunkTop;
                maxY = chunkBottom;
                firstChunk = false;
            } 
            else 
            {
                if (chunkLeft < minX) minX = chunkLeft;
                if (chunkRight > maxX) maxX = chunkRight;
                if (chunkTop < minY) minY = chunkTop;
                if (chunkBottom > maxY) maxY = chunkBottom;
            }
        }
    }

    int mapWidth = (maxX - minX) * tileSize.x;
    int mapHeight = (maxY - minY) * tileSize.y;
    int mapOffsetX = -minX * tileSize.x;  // Offset para normalizar coordenadas
    int mapOffsetY = -minY * tileSize.y;

    // 3. Crear pantalla virtual
    sf::RenderTexture virtualScreen;
    if (!virtualScreen.create(mapWidth, mapHeight)) 
    {
        std::cerr << "Error al crear la pantalla virtual." << std::endl;
        return -1;
    }
    virtualScreen.clear(sf::Color::Transparent);

    // 4. Configurar vista virtual (centrada en el mapa)
    sf::View virtualView(sf::FloatRect(0, 0, mapWidth, mapHeight));
    virtualScreen.setView(virtualView);

    // 5. Dibujar todo el mapa en la pantalla virtual
    for (const auto& layer : layers) 
    {
        tmx::TMXLoader::DrawTileMap(virtualScreen, tileset, layer, tileSize, mapOffsetX, mapOffsetY);
    }
    virtualScreen.display();

    // 6. Configurar cámara principal
    sf::View camera = window.getDefaultView();
    camera.setCenter(mapWidth/2, mapHeight/2); // Centrar en el mapa

    // Bucle principal
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) window.close();
            
            // Zoom con rueda del mouse
            if (event.type == sf::Event::MouseWheelScrolled) 
            {
                if (event.mouseWheelScroll.delta > 0) 
                {
                    camera.zoom(0.9f);
                } 
                else 
                {
                    camera.zoom(1.1f);
                }
            }
        }

        // Movimiento de cámara
        float moveSpeed = 10.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) camera.move(0, -moveSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) camera.move(0, moveSpeed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) camera.move(-moveSpeed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) camera.move(moveSpeed, 0);

        // Renderizado
        window.clear();
        window.setView(camera);
        window.draw(sf::Sprite(virtualScreen.getTexture()));
        window.display();
    }

    return 0;
}