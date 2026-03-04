#pragma once
#include "Application.h"
#include "Maze.h"
#include "GameContext.h"

class GameState : public IState, public IWindowKeeper
{
private:
    Maze m_maze;
    ContextManager m_context_manager;
public:
    GameState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title);
    virtual ~GameState() override = default;

    bool do_step() override;
    void event_handling() override;
    void update() override;
    void render() override;

    void set_maze(Maze&& maze) { m_maze = std::move(maze); }
    void set_context(GameContext&& context) {m_context_manager.reset(std::move(context));}
};
