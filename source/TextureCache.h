#pragma once
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Texture.hpp"

class TextureCache
{
private:
    std::unordered_map<std::string, sf::Texture> m_textures;

public:
    sf::Texture& load(const std::string& texture_path);

};
