#pragma once

#include <src/Bird.hpp>
#include <src/World.hpp>
#include <src/states/BaseState.hpp>

class ModeHard: public BaseGameMode
{
public:

    void handle_inputs(const sf::Event& event) noexcept override{};
};