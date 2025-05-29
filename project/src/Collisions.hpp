#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class CollisionTile {
public:
    CollisionTile();
    void addCollisionTile(const sf::FloatRect& tileRect);
    bool checkCollision(const sf::FloatRect& bounds) const;
    void clear();
    void drawCollisionTiles(sf::RenderWindow& window);
    std::vector<sf::FloatRect>& getCollisionTiles();
    
private:
    std::vector<sf::FloatRect> collisionTiles;
};