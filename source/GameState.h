#pragma once
#include "GameContext.h"

class GameState : public IState, public IWindowKeeper
{
private:
    Maze m_maze;
    ContextManager m_context_manager;
public:
    GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title);
    virtual ~GameState() override;

    bool do_step() override;
    void event_handling() override;
    void update() override;
    void render() override;

    void set_maze(Maze&& maze) { m_maze = std::move(maze); }
    void set_context(GameContext&& context) {m_context_manager.reset(std::move(context));}
};

class IGameEvent
{
public:
    IGameEvent() = default;
    virtual ~IGameEvent() = default;

    virtual void handle(GameContext* context) const = 0;
};

class DeleteStaticEntity : public IGameEvent
{
private:
    IStaticEntity* m_ptr_entity;
public:
    DeleteStaticEntity(IStaticEntity* entity) : m_ptr_entity(entity) {}
    ~DeleteStaticEntity() override = default;
    void handle(GameContext* context) const override;
};

class LostGame : public IGameEvent
{
public:
    void handle(GameContext* context) const override{context->state = GameContext::State::LOST;}
};