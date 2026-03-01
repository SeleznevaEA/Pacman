#include "GameState.h"
#include "SelectState.h"
#include "Resources.h"

GameState::GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title)
    : IState(state_manager)
    , IWindowKeeper(video_mode, window_title) {

    std::cout << "GameState created" << std::endl;
}

bool GameState::do_step() {
    if (!m_window.isOpen()) {
        return false;
    }
    event_handling();
    update();
    render();

    return true;
}

void GameState::event_handling() {
    while (auto event = m_window.pollEvent()) {
        if (auto event_closed = event.value().getIf<sf::Event::Closed>()) {
            // Из GameState можно перейти только в SelectState
            set_next_state(std::make_unique<SelectState>(
                m_state_manager,
                sf::VideoMode({1000, 600}),
                "Select State"
            ));
        }

        if (auto event_pressed = event.value().getIf<sf::Event::KeyPressed>()) {
            if (event_pressed->code == sf::Keyboard::Key::Escape) {
                // Из GameState можно перейти только в SelectState
                set_next_state(std::make_unique<SelectState>(
                    m_state_manager,
                    sf::VideoMode({1000, 600}),
                    "Select State"
                ));
            }
        }
    }
}

void GameState::update() {
    // Здесь будет игровая логика
}

void GameState::render() {
    m_window.clear(sf::Color::Black);

    // Здесь будет отрисовка игры

    m_window.display();
}