#include "Commands.h"
#include "GameState.h"

GameCommand::~GameCommand() {
    delete m_ptr_director;
}

void GameCommand::execute(){
    if (!m_ptr_director) return;
    
    // Директор создаёт GameState через строителя
    GameState* game_state = m_ptr_director->build(m_state_manager);
    
    if (game_state)
        m_state_manager.set_next_state(std::unique_ptr<GameState>(game_state));
}
