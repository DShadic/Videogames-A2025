#include "boy.hpp"

Boy::Boy() :
    animationSpeed(0.1f),   // 0.1 segundos por frame
    currentTime(0.0f),
    currentFrame(0),
    frameWidth(32),         // Ancho de cada frame
    frameHeight(32),        // Altura de cada frame 
    totalFrames(4),
    currentRow(0),           // Fila inicial
    isMoving(false),
    idleRow(0),              // Fila de animación cuando está quieto
    lastMovementRow(0) 
{
}

// Carga la textura del spritesheet
bool Boy::loadTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        return false;
    }
    
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    
    //Centrar el origen para 
    sprite.setOrigin(frameWidth / 2.0f, frameHeight / 2.0f);
    
    return true;
}

// Establece la posición del personaje
void Boy::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// Obtiene la posición actual
sf::Vector2f Boy::getPosition() const {
    return sprite.getPosition();
}

void Boy::setAnimationRow(int row) {
    if(row != currentRow) {
        lastMovementRow = row;  // Guardar última dirección
        currentRow = row;
        currentFrame = 0;
        isMoving = true;        // Indicar que está en movimiento
    }
}

// Dibuja el sprite en la ventana
void Boy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Mueve al personaje
void Boy::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

void Boy::stopMoving() {
    isMoving = false;
    currentFrame = 0;         // Reiniciar al primer frame de la animación
    currentRow = idleRow;     // Usar fila de animación idle
}

void Boy::update(float deltaTime) {
    if (isMoving) {        
        currentTime += deltaTime;        
        if (currentTime >= animationSpeed) {
            currentFrame = (currentFrame + 1) % totalFrames;
            currentTime = 0.0f;
        }
    } else {
        // Mantener el primer frame de la animación idle
        currentFrame = 0;
    }
    
    // Actualizar textura
    int left = currentFrame * frameWidth;
    int top = currentRow * frameHeight;
    sprite.setTextureRect(sf::IntRect(left, top, frameWidth, frameHeight));
}