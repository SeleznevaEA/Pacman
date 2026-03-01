#pragma once
#include "application.h"

class ExitState : public IState
{
public:
    ExitState(IStateManager& state_manager);
    inline bool do_step() override {return false;}
};