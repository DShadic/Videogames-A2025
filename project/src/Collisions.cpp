#include "Collisions.hpp"

CollisionTile::CollisionTile()
{
    collisionTiles = std::vector<sf::FloatRect>();
}

void CollisionTile::addCollisionTile(const sf::FloatRect& tile)
{
    collisionTiles.push_back(tile);
}

bool CollisionTile::checkCollision(const sf::FloatRect& bounds) const 
{
    for (const auto& tile : collisionTiles) {
        if (tile.intersects(bounds)) {
            return true;
        }
    }
    return false;
}

std::vector<sf::FloatRect>& CollisionTile::getCollisionTiles()
{ 
    return collisionTiles; 
}

void CollisionTile::drawCollisionTiles(sf::RenderWindow& window) 
{
    sf::RectangleShape debugRect;
    debugRect.setFillColor(sf::Color(255, 0, 0, 100)); // Este color es rojo para debuguear las colisiones
    
    for (const auto& tile : collisionTiles) 
    {
        debugRect.setPosition(tile.left, tile.top);
        debugRect.setSize(sf::Vector2f(tile.width, tile.height));
        window.draw(debugRect);
    }
}

void CollisionTile::clear() 
{
    collisionTiles.clear();
}