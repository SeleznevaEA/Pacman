#pragma once
#include <string>
#include "Application.h"
#include "GameState.h"

class IGameBuilder;

class GameBuilderDirector{
private:
    std::string m_window_title;
    sf::VideoMode m_mode;
    float m_dynamic_object_ratio;
    IGameBuilder* m_ptr_builder;
public:
    GameBuilderDirector(IGameBuilder* ptr_builder, sf::VideoMode video_mode,
        std::string window_title, float dynamic_object_ratio);
    ~GameBuilderDirector();
    GameState* build(IStateManager& state_manager);
};

class IGameBuilder{
public:
    IGameBuilder() = default;
    virtual ~IGameBuilder() = default;

    virtual void create_rooms()=0;
    virtual void set_room_sides()=0;
    virtual void create_context(float dynamic_object_ratio) =0;
    virtual void create_state(IStateManager& state_manager, sf::VideoMode& video_mode,
        std::string window_title) =0;
    virtual void set_all_to_state()=0;
    virtual GameState* get_game()=0;
};

class CommonBuilder : public IGameBuilder{
protected:
    float m_width;
    float m_height;
    float m_room_size;
    std::vector<std::vector<Room*>> m_rooms;
    GameContext* m_context;
    GameState* m_game_state;

    //maybe?
    bool has_room(int row, int col) const;
    void create_room_at(int row, int col);
    void delete_rooms();

public:
    CommonBuilder(sf::VideoMode video_mode, std::string window_title);
    ~CommonBuilder() override;
    void create_context(float dynamic_object_ratio) override;
    void create_state(IStateManager& state_manager, sf::VideoMode& video_mode,
        std::string window_title) override;
    void set_all_to_state() override;
    GameState* get_game() override;
};

class SimpleBuilder : public CommonBuilder{
public:
    SimpleBuilder(sf::VideoMode video_mode, std::string window_title);
    void create_rooms() override;
    void set_room_sides() override;
};

class ComplexBuilder : public CommonBuilder{
public:
    ComplexBuilder(sf::VideoMode video_mode, std::string window_title);
    void create_rooms() override;
    void set_room_sides() override;
    void ensure_neighbors(int row, int col);
    void add_random_neighbor(int row, int col);
};
