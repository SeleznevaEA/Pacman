#pragma once
#include "ExitState.h"
#include "Menu.h"
#include "Builders.h"

class ChangeStateCommand : public ISelectCommand
{
protected:
    IStateManager& m_state_manager;
public:
    ~ChangeStateCommand() override = default;
    virtual void execute() override = 0;
    explicit ChangeStateCommand(IStateManager& state_manager) : m_state_manager(state_manager){}
};

class GameCommand : public ChangeStateCommand
{
private:
    GameBuilderDirector* m_ptr_director;
public:
    GameCommand(IStateManager& state_manager, GameBuilderDirector* ptr_director)
        : ChangeStateCommand(state_manager), m_ptr_director(ptr_director){}
    
    ~GameCommand() override;
    void execute() override;
};

class ExitCommand : public ISelectCommand
{
private:
    IStateManager& m_state_manager;
public:
    ExitCommand(IStateManager& state_manager) : m_state_manager(state_manager) {}
    void execute() override{
        m_state_manager.set_next_state(std::make_unique<ExitState>(m_state_manager));
    }
};