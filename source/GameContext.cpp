#include "GameContext.h"

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
    for (auto& obj : dynamic_objects)
        if (obj)
            obj->action();
}

void GameContext::check_game_state(){
    if (state != State::INGAME) return;
    bool all_eaten = true;
    for (const auto& obj : static_objects)
        if (auto* food = dynamic_cast<Food*>(obj.get()))
            if (!food->is_eaten()) {all_eaten = false; break;}
    if (all_eaten)
        state = State::WIN;
}

void Enemy::action(){
    auto miliseconds = static_cast<size_t>(m_stopwatch.getElapsedTime().asMilliseconds());
    if (miliseconds < static_cast<size_t>(rand() % 3000)) return;
    if (m_ptr_room){
        auto direction = static_cast<Direction>(rand() % 4);
        m_ptr_room -> get_side(direction) -> enter(this);
    }
    m_stopwatch.restart();
}

void Enemy::prepare_for_drawing(){
    if (m_ptr_room){
        sf::Vector2f room_pos = m_ptr_room->get_position();
        float romm_size = m_ptr_room->get_size();
    }
}


void Food::prepare_for_drawing(){
    if (m_is_eaten || !m_ptr_room) return;
    sf::Vector2f room_pos = m_ptr_room->get_position();
    float food_size = m_ptr_room->get_size();
}

Pacman::Pacman(const Pacman& other){

}
