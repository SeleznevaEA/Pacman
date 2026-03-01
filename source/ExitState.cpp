#include "ExitState.h"

ExitState::ExitState(IStateManager& state_manager) : IState(state_manager){
    std::cout << "ExitState created - application will terminate" << std::endl;
}
