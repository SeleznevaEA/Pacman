#include "Menu.h"
#include "Commands.h"

Menu::Menu(IStateManager& state_manager)
    :m_state_manager(state_manager), m_selected_button(nullptr), m_buttons(), m_title_text(MyFont::instance().get_font()){

}

void Menu::setup_buttons(sf::Vector2u window_size){
    float center_x = window_size.x / 2.0f;
    float button_width = 320.0f;
    float button_height = 60.0f;

    m_title_text.setFont(MyFont::instance().get_font());
    m_title_text.setCharacterSize(35);
    m_title_text.setString("Choose The Difficulty");
    m_title_text.setFillColor(sf::Color::Black);
    m_title_text.setOutlineThickness(3);
    m_title_text.setOutlineColor(sf::Color::White);
    m_title_text.setPosition(sf::Vector2f(80, 100));

    m_buttons[0].set(
        sf::Vector2f{100, 850 - 7*button_height},
        sf::Vector2f{button_width, button_height},
        "5 Fingers",
        30,
        new GameCommand(m_state_manager, new GameBuilderDirector()));

    m_buttons[1].set(
        sf::Vector2f{100, 850 - 5*button_height},
        sf::Vector2f{button_width, button_height},
        "15 Fingers",
        30,
        new GameCommand(m_state_manager, new GameBuilderDirector()));

    m_buttons[2].set(
        sf::Vector2f{100, 850 - 3*button_height},
        sf::Vector2f{button_width, button_height},
        "20 Fingers",
        30,
        new GameCommand(m_state_manager, new GameBuilderDirector()));

    m_buttons[3].set(
        sf::Vector2f{100, 850},
        sf::Vector2f{button_width, button_height},
        "Exit",
        30,
        new ExitCommand(m_state_manager)
    );
}

void Menu::process_mouse(sf::Vector2f mouse_pos, bool is_pressed){
    Button* selected = nullptr;
    for (auto& button : m_buttons){
        if (button.is_position_in(mouse_pos)){
            selected = &button;
            break;
        }
    }
    if (selected != m_selected_button){
        if (m_selected_button)
            m_selected_button->unselect();
        m_selected_button = selected;
        if (m_selected_button)
            m_selected_button->select();
    }
    if (is_pressed && m_selected_button)
        m_selected_button->push();
}
void Menu::select_next(){
    if (m_buttons.empty()) return;
    if (!m_selected_button){
        m_selected_button = &m_buttons[0];
        m_selected_button->select();
        return;
    }
    for (size_t i = 0; i < m_number_of_buttons; ++i){
        if (&m_buttons[i] == m_selected_button){
            m_selected_button->unselect();
            size_t next_index = (i+1)%m_number_of_buttons;
            m_selected_button = &m_buttons[next_index];
            m_selected_button->select();
            return;
        }
    }
}

void Menu::select_prev(){
    if (m_buttons.empty()) return;
    if (!m_selected_button){
        m_selected_button = &m_buttons.back();
        m_selected_button->select();
        return;
    }
    for (size_t i = 0; i < m_number_of_buttons; ++i){
        if (&m_buttons[i] == m_selected_button){
            m_selected_button->unselect();
            size_t prev_index = (i==0) ? m_number_of_buttons-1 : i-1;
            m_selected_button = &m_buttons[prev_index];
            m_selected_button->select();
            return;
        }
    }
}

void Menu::draw_into(sf::RenderWindow& window) const{
    window.draw(m_title_text);
    for (const auto& button : m_buttons)
        button.draw_into(window);
}
