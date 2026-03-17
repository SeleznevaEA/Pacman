#include "SelectState.h"

SelectState::SelectState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title)
    : IState(state_manager), IWindowKeeper(video_mode, window_title), m_menu(std::make_unique<Menu>(state_manager)),
    m_background(Resources::BackgroundTexture()) {
    auto window_size = m_window.getSize();
    
    // Проверяем, что текстура валидна
    if (m_background.getTexture().getSize().x > 0) {
        auto texture_size = m_background.getTexture().getSize();
        float scale_x = float(window_size.x) / texture_size.x;
        float scale_y = float(window_size.y) / texture_size.y;
        m_background.setScale({scale_x, scale_y});
    }

    m_menu->setup_buttons(window_size);
}

bool SelectState::do_step(){
    if (!m_window.isOpen()) return false;
    event_handling();
    update();
    render();
    return true;
}

void SelectState::event_handling(){
    while (auto event = m_window.pollEvent()){
        if (auto event_closed = event.value().getIf<sf::Event::Closed>()){
            set_next_state(std::make_unique<ExitState>(m_state_manager));
        }
        if (auto event_resized = event.value().getIf<sf::Event::Resized>()) {
            sf::View view = m_window.getView();
            view.setSize(sf::Vector2f(event_resized->size.x, event_resized->size.y));
            m_window.setView(view);
        }
        if (auto event_pressed = event.value().getIf<sf::Event::KeyPressed>()){
            if (event_pressed->code == sf::Keyboard::Key::Up){
                m_menu -> select_prev();
                break;
            }
            if (event_pressed->code == sf::Keyboard::Key::Down){
                m_menu -> select_next();
                break;
            }
        //     // if (event_pressed->code == sf::Keyboard::Key::Space){
        //     //     if (auto selected = m_menu-> get_selected_button())
        //     //         selected -> push();
        //     //     break;
        //     //}
        }
    }
}

void SelectState::update(){
    sf::Vector2i mouse_pixel_pos = sf::Mouse::getPosition(m_window);
    sf::Vector2f mouse_global_pos = m_window.mapPixelToCoords(mouse_pixel_pos);
    bool is_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    m_menu->process_mouse(mouse_global_pos, is_pressed);
}

void SelectState::render(){
    m_window.clear();
    m_window.draw(m_background);
    m_menu->draw_into(m_window);
    m_window.display();
}