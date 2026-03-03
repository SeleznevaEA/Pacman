#include "TextureCache.h"

sf::Texture& TextureCache::load(const std::string& texture_path){
    auto it = m_textures.find(texture_path);
    if(it == m_textures.end())
    {
        sf::Texture texture;
        if (!texture.loadFromFile(texture_path))
            throw std::runtime_error("Texture could not be loaded: " + texture_path);
        auto result = m_textures.emplace(texture_path, std::move(texture));
        it = result.first;
    }
    return it->second;
}