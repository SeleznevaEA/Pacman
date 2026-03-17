#pragma once
#include "Application.h"

class Resources {
public:
    static sf::Texture& BackgroundTexture() {
        static sf::Texture texture{ "assets/menu_background.jpg" };
        return texture;
    }
    static sf::Texture& GojoMainTexture() {
        static sf::Texture texture{ "assets/gojo/idle/0.png" };
        return texture;
    }
    static sf::Texture& SukunaMainTexture() {
        static sf::Texture texture{ "assets/sukuna/idle/0.png" };
        return texture;
    }
    static sf::Texture& Food() {
        static sf::Texture texture{ "assets/berry.jpg" };
        return texture;
    }
    static sf::Texture& Win() {
        static sf::Texture texture{ "assets/gojo_win.jpg" };
        return texture;
    }
    static sf::Texture& Loose() {
        static sf::Texture texture{ "assets/sukuna_win.jpg" };
        return texture;
    }
    static sf::Texture& SukRight() {
        static sf::Texture texture{ "assets/sukuna/right/0.png" };
        return texture;
    }
    static sf::Texture& SukLeft() {
        static sf::Texture texture{ "assets/sukuna/left/0.png" };
        return texture;
    }
    static sf::Texture& GojoRight0() {
        static sf::Texture texture{ "assets/gojo/right/0.png" };
        return texture;
    }
    static sf::Texture& GojoRight1() {
        static sf::Texture texture{ "assets/gojo/right/1.png" };
        return texture;
    }
    static sf::Texture& GojoRight2() {
        static sf::Texture texture{ "assets/gojo/right/2.png" };
        return texture;
    }
    static sf::Texture& GojoRight3() {
        static sf::Texture texture{ "assets/gojo/right/3.png" };
        return texture;
    }
    static sf::Texture& GojoLeft0() {
        static sf::Texture texture{ "assets/gojo/left/0.png" };
        return texture;
    }
    static sf::Texture& GojoLeft1() {
        static sf::Texture texture{ "assets/gojo/left/1.png" };
        return texture;
    }
    static sf::Texture& GojoLeft2() {
        static sf::Texture texture{ "assets/gojo/left/2.png" };
        return texture;
    }
    static sf::Texture& GojoLeft3() {
        static sf::Texture texture{ "assets/gojo/left/3.png" };
        return texture;
    }
public:
    Resources(const Resources& obj) = delete;
    Resources& operator =(const Resources& obj) = delete;
private:
    Resources() = default;
};