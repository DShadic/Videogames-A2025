#pragma once

#include <src/Bird.hpp>
#include <src/World.hpp>
#include <src/states/BaseState.hpp>

class PauseState: public BaseState 
{
    public: 
        PauseState(StateMachine* sm) noexcept;
        void enter(std::shared_ptr<World> _world = nullptr, std::shared_ptr<Bird> _bird = nullptr) noexcept override;
        //void exit();
        void handle_inputs(const sf::Event& event)noexcept override;
        void render(sf::RenderTarget& target) const noexcept override;

    private:
    std::shared_ptr<World> world;
    std::shared_ptr<Bird> bird;
    //int score;
    bool paused;
    //std::shared_ptr<BaseState> current_state{std::make_shared<BaseState>(this)};
};