#include "GameState.h"
#include "SelectState.h"
#include "Resources.h"

GameState::GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title)
    : IState(state_manager)
    , IWindowKeeper(video_mode, window_title)
    , m_maze(){}

GameState::~GameState() {
}

bool GameState::do_step() {
    event_handling();
    
    // Если окно закрыто (переход в SelectState), возвращаем true для применения нового состояния
    if (!m_window.isOpen()) {
        return true;
    }
    
    update();
    render();

    return true;
}

void GameState::event_handling() {
    while (auto event = m_window.pollEvent()) {
        if (auto event_closed = event.value().getIf<sf::Event::Closed>()) {
            // Создаём SelectState, окно GameState закроется автоматически при уничтожении
            set_next_state(std::make_unique<SelectState>(
                m_state_manager,
                sf::VideoMode({963, 1020}),
                "Game Menu"
            ));
            m_window.close();
            return;
        }

        if (auto event_pressed = event.value().getIf<sf::Event::KeyPressed>()) {
            if (event_pressed->code == sf::Keyboard::Key::Escape) {
                // Создаём SelectState, окно GameState закроется автоматически при уничтожении
                set_next_state(std::make_unique<SelectState>(
                    m_state_manager,
                    sf::VideoMode({963, 1020}),
                    "Game Menu"
                ));
                m_window.close();
                return;
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