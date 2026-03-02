#include "Commands.h"
#include "GameState.h"
void GameCommand::execute(){
    m_state_manager.set_next_state(std::make_unique<GameState>(m_state_manager,
        sf::VideoMode({1250,1250}), "Game level"));
}