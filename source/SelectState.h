#pragma once
#include "Menu.h"
#include "Resources.h"
#include "ExitState.h"


class SelectState : public IState, public IWindowKeeper
{
private:
    std::unique_ptr<Menu> m_menu;
    sf::Sprite m_background;
public:
    SelectState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title);
    virtual ~SelectState() override = default;

    bool do_step() override;
    void event_handling() override;
    void update() override;
    void render() override;
};

