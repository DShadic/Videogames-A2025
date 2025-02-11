#include <Settings.hpp>
#include <src/text_utilities.hpp>
#include <src/states/StateMachine.hpp>
#include <src/Modes/BaseGameMode.hpp>
#include <src/Modes/ModeHard.hpp>

ModeHard::ModeHard() noexcept 
{

}

void ModeHard::handle_inputs(const sf::Event& event, std::shared_ptr<Bird> bird) noexcept

{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        bird->jump();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
    {
        bird->moveLeft();
    }
    
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
    {
        bird->moveRight();
    }

}

void ModeHard::

