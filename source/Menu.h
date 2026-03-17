#pragma once
#include "Application.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include <SFML/Audio.hpp>

class ISelectCommand;

class IDrawable
{
public:
    IDrawable() = default;
    virtual ~IDrawable() = default;
    virtual void draw_into(sf::RenderWindow& window) const = 0;
};

class Button : public IDrawable
{
private:
    sf::Text m_text;
    sf::RectangleShape m_rectangle;
    ISelectCommand* m_ptr_command;
public:
    Button();
    ~Button() override;
    void set(sf::Vector2f pos, sf::Vector2f button_size, const std::string& text,
        size_t font_size, ISelectCommand* ptr_command);
    void select();
    void unselect();
    bool is_position_in(sf::Vector2f pos) const;
    void push();
    void draw_into(sf::RenderWindow& window) const override;
};

class Menu : public IDrawable
{
private:
    static constexpr size_t m_number_of_buttons = 4;
    std::array<Button, m_number_of_buttons> m_buttons;
    IStateManager& m_state_manager;
    Button* m_selected_button;
    sf::Text m_title_text;
public:
    explicit Menu(IStateManager& state_manager);
    ~Menu() override = default;

    void process_mouse(sf::Vector2f mouse_pos, bool is_pressed);
    void setup_buttons(sf::Vector2u window_size);
    void select_next();
    void select_prev();
    Button* get_selected_button() const {return m_selected_button;}
    void draw_into(sf::RenderWindow& window) const override;
};

class ISelectCommand
{
public:
    ISelectCommand() = default;
    virtual ~ISelectCommand() = default;

    virtual void execute() = 0;
};

class MyFont
{
private:
    sf::Font m_font{ "assets/PacManSenior-Regular.otf" };

    MyFont() = default;
public:
    MyFont(const MyFont&) = delete;
    MyFont& operator=(const MyFont&) = delete;
    ~MyFont() = default;
    static MyFont& instance();

    sf::Font& get_font() {return m_font;}
    const sf::Font& get_font() const {return m_font;}
};
