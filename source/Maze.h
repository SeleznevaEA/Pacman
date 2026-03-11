#pragma once
#include "Menu.h"

enum Direction { INVALID = -1, LEFT, RIGHT, UP, DOWN };
class IEntity;

class IPreparable : public IDrawable
{
public:
    virtual void prepare_for_drawing() =0;
};

class IRoomSide : public IPreparable
{
public:
    virtual void enter(IEntity* entity) =0;
    virtual ~IRoomSide() override = default;
};

class Room : public IDrawable
{
private:
    sf::RectangleShape m_rectangle;
    int m_enemy_count = 0;
public:
    std::array<IRoomSide*, 4> m_sides{nullptr, nullptr, nullptr, nullptr};
public:
    Room(float size);
    ~Room() override {};

    float get_size() const {return m_rectangle.getSize().x;}
    void set_position(sf::Vector2f pos){m_rectangle.setPosition(pos);}
    sf::Vector2f get_position() const {return m_rectangle.getPosition();}

    void set_side(Direction side, IRoomSide* ptr_side);
    IRoomSide* get_side(Direction side) const;
    Direction get_direction(IRoomSide* ptr_side) const;
    void draw_into(sf::RenderWindow& window) const override;

    bool has_enemy() const {if (m_enemy_count >= 2) {std::cout<<m_enemy_count;} return m_enemy_count > 0;}
    void add_enemy() {m_enemy_count += 1;}
    void remove_enemy() {m_enemy_count-=1;}
};

class IEntity : virtual public IPreparable
{
protected:
    Room* m_ptr_room;
public:
    IEntity() : m_ptr_room(nullptr) {}
    ~IEntity() override = default;

    void set_location(Room* ptr_room) { m_ptr_room= ptr_room;prepare_for_drawing();}
    Room* get_ptr_location() const {return m_ptr_room;}
};

class Maze : public IDrawable
{
private:
    std::vector<Room*> m_rooms;
public:
    Maze() = default;
    explicit Maze(std::vector<Room*>& rooms);
    ~Maze() override;
    
    Maze(Maze&& other) noexcept : m_rooms(std::move(other.m_rooms)) {}
    Maze& operator=(Maze&& other) noexcept;
    
    // Запрещаем копирование
    Maze(const Maze&) = delete;
    Maze& operator=(const Maze&) = delete;

    void draw_into(sf::RenderWindow& window) const override;
    void update_entities(const std::vector<IEntity*>& entities);
};

class Pass : public IRoomSide
{
private:
    Room& m_room1;
    Room& m_room2;
public:
    Pass(Room& room1, Room& room2) : m_room1(room1), m_room2(room2) {}

    void enter(IEntity* entity) override;
    void prepare_for_drawing() override{}
    void draw_into(sf::RenderWindow& window) const override{}
};

class Wall : public IRoomSide
{
private:
    Room& m_room;
    sf::Vertex m_line[2];
public:
    Wall(Room& room) : m_room(room) {}
    void enter(IEntity* entity) override{}
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
};