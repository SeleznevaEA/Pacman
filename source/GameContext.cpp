#include "GameContext.h"
#include "Resources.h"
#include "GameState.h"

GameContext::GameContext(const GameContext& other){
    if (other.pacman) pacman = std::make_unique<Pacman>(*other.pacman);
    for (const auto& obj : other.static_objects)
        if (obj)
            static_objects.push_back(obj->clone());
    for (const auto& obj : other.dynamic_objects)
        if (obj)
            dynamic_objects.push_back(obj->clone());
    state = other.state;
}

GameContext& GameContext::operator=(const GameContext& other){
    if (this == &other) return *this;
    clear();
    if (other.pacman) pacman = std::make_unique<Pacman>(*other.pacman);
    for (const auto& obj : other.static_objects)
        if (obj)
            static_objects.push_back(obj->clone());
    for (const auto& obj : other.dynamic_objects)
        if (obj)
            dynamic_objects.push_back(obj->clone());
    state = other.state;
    return *this;
}

void GameContext::clear(){
    pacman.reset();
    static_objects.clear();
    dynamic_objects.clear();
    state = State::INGAME;
}

void GameContext::update(){
    check_eat();
    for (auto& obj : dynamic_objects)
        if (obj)
            obj->action();
}

void GameContext::check_game_state(){
    if (state != State::INGAME) return;

    if (score == static_objects.size())
    {
        state = State::WIN;
        std::cout << "Score: " << score << std::endl;
    }
}

void GameContext::check_eat(){
    for (auto& food : static_objects)
    {
        if (pacman->get_ptr_location() == food->get_ptr_location())
        {
            food->set_eaten(true);
            increment_score();
        }
    }
}

Enemy::Enemy() : m_sprite(Resources::SukunaMainTexture()){}

void Enemy::action(){
    m_move_accumulator += m_stopwatch.restart().asSeconds();
    while (m_move_accumulator >= m_move_interval) {
        if (m_ptr_room) {
            auto direction = static_cast<Direction>(rand() % 4);
            m_ptr_room->get_side(direction)->enter(this);

        }
        m_move_accumulator -= m_move_interval;
    }
}

void Enemy::prepare_for_drawing(){
    if (!m_ptr_room) return;
    
    sf::Vector2f room_pos = m_ptr_room->get_position();
    float room_size = m_ptr_room->get_size();
    
    auto tex_size = m_sprite.getTexture().getSize();
    float target_size = room_size * 0.8f;
    float scale = target_size / std::max(tex_size.x, tex_size.y);
    m_sprite.setScale({scale, scale});
    
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    m_sprite.setPosition({
        room_pos.x + (room_size - bounds.size.x) / 2,
        room_pos.y + (room_size - bounds.size.y) / 2
    });
}

void Enemy::draw_into(sf::RenderWindow& window) const {
    if (!m_ptr_room) return;
    window.draw(m_sprite);
}

Food::Food() : m_sprite(Resources::Food()){}

void Food::prepare_for_drawing(){
    if (!m_ptr_room) return;
    
    sf::Vector2f room_pos = m_ptr_room->get_position();
    float room_size = m_ptr_room->get_size();
    
    // Масштабируем по размеру текстуры
    auto tex_size = m_sprite.getTexture().getSize();
    float target_size = room_size * 0.25f;
    float scale = target_size / std::max(tex_size.x, tex_size.y);
    m_sprite.setScale({scale, scale});
    
    // Центрируем
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    m_sprite.setPosition({
        room_pos.x + (room_size - bounds.size.x) / 2,
        room_pos.y + (room_size - bounds.size.y) / 2
    });
}


void Food::draw_into(sf::RenderWindow& window) const {
    if (!m_is_eaten && m_ptr_room)
        window.draw(m_sprite);
}

Pacman::Pacman() : m_sprite(Resources::GojoMainTexture()){}

void Pacman::move(Direction direction){
    if (m_ptr_room)
        if (auto* side = m_ptr_room->get_side(direction))
            side -> enter(this);
}

void Pacman::prepare_for_drawing() {
    if (!m_ptr_room) return;
    
    sf::Vector2f room_pos = m_ptr_room->get_position();
    float room_size = m_ptr_room->get_size();
    
    // Масштабируем по размеру текстуры
    auto tex_size = m_sprite.getTexture().getSize();
    float target_size = room_size * 0.9f;
    float scale = target_size / std::max(tex_size.x, tex_size.y);
    m_sprite.setScale({scale, scale});
    
    // Центрируем в комнате
    sf::FloatRect bounds = m_sprite.getGlobalBounds();
    m_sprite.setPosition({
        room_pos.x + (room_size - bounds.size.x) / 2,
        room_pos.y + (room_size - bounds.size.y) / 2
    });
}

void Pacman::draw_into(sf::RenderWindow& window) const {
    if (!m_ptr_room) return;
    window.draw(m_sprite);
}


void ContextManager::restore_previous_context() {
    if (m_contexts.size() > 1)
        m_contexts.pop();
    else
        m_contexts.top() = m_initial_context.clone();
}

void ContextManager::reset(GameContext&& initial_context){
    m_initial_context = initial_context;
    m_contexts = std::stack<GameContext>();
    save_current_context();
}