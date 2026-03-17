#include "GameState.h"

#include <algorithm>

#include "SelectState.h"
#include "Resources.h"

GameState::GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title)
    : IState(state_manager)
    , IWindowKeeper(video_mode, window_title)
    , m_maze()
    ,m_win_buffer("assets/gojo/nah.wav")
    ,m_lose_buffer("assets/gojo/sukuna_laugh.wav")
    , m_sound(m_win_buffer)
    , m_sound_played(false){}

GameState::~GameState() {
    m_sound.stop();
}

bool GameState::do_step() {

    event_handling();

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
            if (event_pressed->code == sf::Keyboard::Key::A || event_pressed->code == sf::Keyboard::Key::Left) {
                m_context_manager.get_current_context().pacman->move(LEFT);
                return;
            }
            if (event_pressed->code == sf::Keyboard::Key::D || event_pressed->code == sf::Keyboard::Key::Right) {
                m_context_manager.get_current_context().pacman->move(RIGHT);
                return;
            }
            if (event_pressed->code == sf::Keyboard::Key::W || event_pressed->code == sf::Keyboard::Key::Up) {
                m_context_manager.get_current_context().pacman->move(UP);
                return;
            }
            if (event_pressed->code == sf::Keyboard::Key::S || event_pressed->code == sf::Keyboard::Key::Down) {
                m_context_manager.get_current_context().pacman->move(DOWN);
                return;
            }
        }

    }
}

void GameState::update() {
    auto& context = m_context_manager.get_current_context();
    
    context.update();

    if (context.get_pacman()) {
        context.get_pacman()->prepare_for_drawing();
    }
    
    for (const auto& obj : context.get_static_objects()) {
        if (obj) obj->prepare_for_drawing();
    }
    
    for (const auto& obj : context.get_dynamic_objects()) {
        if (obj) obj->prepare_for_drawing();
    }

}

void GameState::render() {

    auto& context = m_context_manager.get_current_context();
    if (context.get_state() == GameContext::State::WIN) {
        sf::Sprite winSprite(Resources::Win());
        sf::Text win_text{MyFont::instance().get_font(), "Nah, I'd WIN"};

        sf::Vector2u windowSize = m_window.getSize();
        sf::Vector2u textureSize = Resources::Win().getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        winSprite.setScale({scaleX, scaleY});

        win_text.setCharacterSize(50);
        win_text.setFillColor(sf::Color::Black);
        win_text.setOutlineThickness(3);
        win_text.setOutlineColor(sf::Color::Blue);
        win_text.setPosition(sf::Vector2f(80, 100));

        m_window.draw(winSprite);
        m_window.draw(win_text);
        m_window.display();

        if (!m_sound_played)
        {
            m_sound_played = true;
            m_sound.setBuffer(m_win_buffer);
            m_sound.setLooping(false);
            m_sound.play();
            m_sound.setVolume(140);
            m_sound_played = true;
            return;
        }

        return;
    }
    if (context.get_state() == GameContext::State::LOST) {
        sf::Sprite loseSprite(Resources::Loose());
        sf::Text loose_text{MyFont::instance().get_font(), "HAHAHAHAH"};
        // Масштабируем под размер окна
        sf::Vector2u windowSize = m_window.getSize();
        sf::Vector2u textureSize = Resources::Loose().getSize();

        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

        loseSprite.setScale({scaleX, scaleY});

        loose_text.setCharacterSize(50);
        loose_text.setFillColor(sf::Color::Black);
        loose_text.setOutlineThickness(3);
        loose_text.setOutlineColor(sf::Color::Red);
        loose_text.setPosition({m_window.getSize().x / 2.0f, m_window.getSize().y / 5.0f});

        m_window.draw(loseSprite);
        m_window.draw(loose_text);
        m_window.display();
        if (!m_sound_played)
        {
            m_sound_played = true;
            m_sound.setBuffer(m_lose_buffer);
            m_sound.setLooping(false);
            m_sound.setVolume(80);
            m_sound.play();
            m_sound_played = true;
            return;
        }
        return;
    }
    else {m_window.clear(sf::Color::Black);}

    m_maze.draw_into(m_window);
    

    for (const auto& obj : context.get_static_objects()) {
        if (obj) obj->draw_into(m_window);
    }
    
    for (const auto& obj : context.get_dynamic_objects()) {
        if (obj) obj->draw_into(m_window);
    }
    
    if (context.get_pacman()) {
        context.get_pacman()->draw_into(m_window);
    }

    m_window.display();
}

void DeleteStaticEntity::handle(GameContext* context) const{
    auto& objects = context->static_objects;
    auto it = std::find_if(objects.begin(), objects.end(),
        [this](const auto& ptr) {
            return ptr.get() == m_ptr_entity;
        });

    if (it != objects.end()) {
        objects.erase(it);
    }
}
