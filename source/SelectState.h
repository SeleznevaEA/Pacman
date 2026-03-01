#pragma once
#include "application.h"

class SelectState : public IState, public IWindowKeeper
{
private:
    enum class SelectOption{
        Start,
        Exit
    };
    SelectOption m_select_option;
    sf::Text m_title_text;
    sf::Text m_start_text;
    sf::Text m_exit_text;
    sf::Font m_font;
public:
    SelectState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title);
    virtual ~SelectState() override = default;

    bool do_step() override;
    void event_handling() override;
    void update() override;
    void render() override;
private:
    void load_font();
    void setup_texts();
    void move_selection_up();
    void move_selection_down();
    void confirm_selection();
};

