#include "Menu.h"
#include <stdexcept>

MyFont& MyFont::instance()
{
    static MyFont font;
    return font;
}

Button::Button() : m_ptr_command(nullptr), m_text(MyFont::instance().get_font()){
    m_rectangle.setFillColor(sf::Color(205, 92,92));
    m_rectangle.setOutlineColor(sf::Color::White);
    m_rectangle.setOutlineThickness(2);
}

void Button::set(sf::Vector2f pos, sf::Vector2f button_size, const std::string& text,
        size_t font_size, ISelectCommand* ptr_command)
{
    m_ptr_command = ptr_command;
    m_rectangle.setPosition(pos);
    m_rectangle.setSize(button_size);

    m_text.setString(text);
    m_text.setCharacterSize(font_size);
    m_text.setFillColor(sf::Color::White);

    sf::FloatRect text_bounds = m_text.getGlobalBounds();
    m_text.setPosition({pos.x + (button_size.x - text_bounds.size.x)/2,
        pos.y + (button_size.y - text_bounds.size.y)/2 - 5});
}

void Button::draw_into(sf::RenderWindow& window) const{
    window.draw(m_rectangle);
    window.draw(m_text);
}

void Button::select(){
    m_rectangle.setFillColor(sf::Color(30,144,255));
}

void Button::unselect(){
    m_rectangle.setFillColor(sf::Color(205, 92,92));
}

bool Button::is_position_in(sf::Vector2f pos) const{
    return m_rectangle.getGlobalBounds().contains(pos);
}

void Button::push(){
    if (m_ptr_command)
        m_ptr_command->execute();
}
