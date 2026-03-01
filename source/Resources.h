#pragma once
#include "application.h"

class Resources {
public:
    static sf::Texture& GojoTexture() {
        static sf::Texture texture{ "assets/Gojo/original/0-0.png" };
        return texture;
    }
    static sf::Font& MainFont() {
        static sf::Font font{ "assets/PacManSenior-Regular.otf" };
        return font;
    }
public:
    Resources(const Resources& obj) = delete;
    Resources& operator =(const Resources& obj) = delete;
private:
    Resources() = default;
};