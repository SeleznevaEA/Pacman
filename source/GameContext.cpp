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
    // TODO: Реализовать подготовку для отрисовки
}

void Enemy::draw_into(sf::RenderWindow& window) const {
    // TODO: Реализовать отрисовку врага
}


void Food::prepare_for_drawing(){
    // TODO: Реализовать подготовку для отрисовки
}

void Food::draw_into(sf::RenderWindow& window) const {
    // TODO: Реализовать отрисовку еды
}

void Pacman::move(Direction direction){
    if (m_ptr_room)
        if (auto* side = m_ptr_room->get_side(direction))
            side -> enter(this);
}

void Pacman::prepare_for_drawing(){
    // TODO: Реализовать отрисовку без анимации
}

void Pacman::draw_into(sf::RenderWindow& window) const {
    // TODO: Реализовать отрисовку без анимации
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