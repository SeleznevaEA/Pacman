#include "SelectState.h"
#include "ExitState.h"
#include <iostream>

int main() {
    try {
        Application app;
        // Устанавливаем начальное состояние - SelectState
        app.set_initial_state(std::make_unique<SelectState>(
            app,
            sf::VideoMode({963, 1020}),
            "Game Menu"
        ));
        
        std::cout << "Application started" << std::endl;
        int result = app.run();
        std::cout << "Application finished with code: " << result << std::endl;
        
        return result;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}