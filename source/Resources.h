#pragma once
#include "Application.h"

class Resources {
public:
    static sf::Texture& BackgroundTexture() {
        static sf::Texture texture{ "assets/menu_background.jpg" };
        return texture;
    }
    static sf::Texture& GojoMainTexture() {
        static sf::Texture texture{ "assets/Gojo/original/0-0.png" };
        return texture;
    }
public:
    Resources(const Resources& obj) = delete;
    Resources& operator =(const Resources& obj) = delete;
private:
    Resources() = default;
};