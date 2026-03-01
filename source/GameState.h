#pragma once
#include "application.h"

class GameState : public IState, public IWindowKeeper
{
public:
    GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title);
    virtual ~GameState() override = default;

    bool do_step() override;
    void event_handling() override;
    void update() override;
    void render() override;
};