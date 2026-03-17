#include "SelectState.h"
#include "ExitState.h"
#include <iostream>
#include <SFML/Audio.hpp>

int main() {
    try {
        sf::SoundBuffer buffer1, buffer2;
        if (!buffer1.loadFromFile("assets/gojo/0-6.wav") || !buffer2.loadFromFile("assets/gojo/0-2.wav")) {
            std::cerr << "Failed to load gojo sound" << std::endl;
            return -1;
        }
        sf::Sound sound(buffer1);
        sound.play();

        while (sound.getStatus() == sf::Sound::Status::Playing) {
            sf::sleep(sf::milliseconds(100));
        }

        Application app;
        app.set_initial_state(std::make_unique<SelectState>(
            app,
            sf::VideoMode({963, 1020}),
            "Game Menu"
        ));
        sound.setBuffer(buffer2);
        sound.play();

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