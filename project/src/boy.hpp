#ifndef BOY_HPP
#define BOY_HPP

#include <SFML/Graphics.hpp>

class Boy {
private:
    sf::Sprite sprite;
    sf::Texture texture;    
    // Parámetros de animación
    float animationSpeed;
    float currentTime;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int currentRow;
    bool isMoving;           // Estado de movimiento
    int idleRow;             // Fila de animación cuando está quieto
    int lastMovementRow;     // Última dirección de movimiento
    

public:
    Boy();
    
    bool loadTexture(const std::string& filename);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void move(float offsetX, float offsetY);
    //cambiar animaciones
    void setAnimationRow(int row);     
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void stopMoving();
    //int getLastMovementRow() const { return lastMovementRow; }
};

#endif