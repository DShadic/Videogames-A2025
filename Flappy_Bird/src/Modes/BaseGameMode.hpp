#pragma once

#include <src/Bird.hpp>
#include <src/World.hpp>
#include <src/states/BaseState.hpp>

class GameMode
{
public:

    virtual void handle_inputs(const sf::Event& event) noexcept {};

    

    
private:

    std::shared_ptr<Bird> bird;
    std::shared_ptr<World> world;
    int score{0};
}