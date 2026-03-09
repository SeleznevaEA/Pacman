#include "Maze.h"

#include "GameContext.h"

Room::Room(float size)
{
    m_rectangle.setSize(sf::Vector2f(size, size));
    m_rectangle.setFillColor(sf::Color::Black);
    m_rectangle.setOutlineColor(sf::Color::White);
    m_rectangle.setOutlineThickness(1);
}


void Room::set_side(Direction direction, IRoomSide* ptr_side){
    if (direction >= 0 && direction < 4){
        if (m_sides[direction])
            delete m_sides[direction];
    }
    m_sides[direction] = ptr_side;
    if (ptr_side)
        ptr_side -> prepare_for_drawing();
}

IRoomSide* Room::get_side(Direction direction) const{
    if (direction >= 0 && direction < 4)
        return m_sides[direction];
    return nullptr;
}

Direction Room::get_direction(IRoomSide* ptr_side) const {
    for (int i = 0; i < 4; ++i) {
        if (m_sides[i] == ptr_side) {
            return static_cast<Direction>(i);
        }
    }
    return Direction::INVALID;
}

void Room::draw_into(sf::RenderWindow& window) const{
    window.draw(m_rectangle);
    for (auto* side : m_sides)
        if (side)
            side -> draw_into(window);
}
void Wall::prepare_for_drawing(){
    sf::Vector2f room_pos = m_room.get_position();
    float room_size = m_room.get_size();
    Direction dir = m_room.get_direction(this);
    sf::Vector2f start, end;
    switch (dir)
    {
    case LEFT:
        start = room_pos;
        end = sf::Vector2f(room_pos.x, room_pos.y+ room_size);
        break;
    case RIGHT:
        start = sf::Vector2f(room_pos.x + room_size, room_pos.y);
        end = sf::Vector2f(room_pos.x + room_size, room_pos.y+ room_size);
        break;
    case UP:
        start = room_pos;
        end = sf::Vector2f(room_pos.x + room_size, room_pos.y);
        break;
    case DOWN:
        start = sf::Vector2f(room_pos.x, room_pos.y + room_size);
        end = sf::Vector2f(room_pos.x + room_size, room_pos.y + room_size);
        break;
    }
    m_line[0].position = start;
    m_line[0].color = sf::Color::Blue;
    m_line[1].position = end;
    m_line[1].color = sf::Color::Blue;
}

void Wall::draw_into(sf::RenderWindow& window) const{
    window.draw(m_line, 2, sf::PrimitiveType::Lines);
}

void Pass::enter(IEntity* entity){
    if (!entity) return;
    Room* current_room = entity->get_ptr_location();
    if (!current_room) return;
    if (current_room == &m_room1)
        entity->set_location(&m_room2);
    else if (current_room == &m_room2)
        entity->set_location(&m_room1);
    // if (auto* Pacman = dynamic_cast<::Pacman*>(entity)){
    //
    // }
}

Maze::Maze(std::vector<Room*>& rooms) : m_rooms(rooms){}

Maze::~Maze(){
    // Сначала удаляем все стороны комнат, потом сами комнаты
    for (size_t idx = 0; idx < m_rooms.size(); ++idx) {
        auto* room = m_rooms[idx];
        if (room) {
            // Очищаем стороны комнаты
            for (size_t i = 0; i < room->m_sides.size(); ++i) {
                if (room->m_sides[i]) {
                    delete room->m_sides[i];
                    room->m_sides[i] = nullptr;
                }
            }
        }
    }

    // Теперь удаляем сами комнаты
    for (size_t idx = 0; idx < m_rooms.size(); ++idx) {
        if (m_rooms[idx]) {
            delete m_rooms[idx];
            m_rooms[idx] = nullptr;
        }
    }
}

void Maze::draw_into(sf::RenderWindow& window) const{
    for (auto* room : m_rooms)
        if (room)
            room->draw_into(window);
}

void Maze::update_entities(const std::vector<IEntity*>& entities){
    for (auto entity : entities)
        if (entity)
            entity->prepare_for_drawing();
}
