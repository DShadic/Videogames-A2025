#pragma once

#include <src/Bird.hpp>
#include <src/World.hpp>
#include <src/states/BaseState.hpp>

class PauseState: public BaseState 
{
    public: 
        PauseState(StateMachine* sm) noexcept;

        void enter(std::shared_ptr<World> _world = nullptr, std::shared_ptr<Bird> _bird = nullptr) noexcept override;
        void exit();
        void handle_input();
        void update(float dt) noexcept override;
        void render();

};