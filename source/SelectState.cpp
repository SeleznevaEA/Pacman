#include "SelectState.h"
#include "Resources.h"
#include "GameState.h"
#include "ExitState.h"

SelectState::SelectState(IStateManager& state_manager, sf::VideoMode video_mode, const std::string& window_title)
    : IState(state_manager), IWindowKeeper(video_mode, window_title), m_select_option(SelectOption::Start),
        m_title_text(Resources::MainFont(), "GAME MENU", 50),
        m_start_text(Resources::MainFont(), "Start Game", 30),
        m_exit_text(Resources::MainFont(), "Exit", 30),
        m_font(Resources::MainFont())
{
    load_font();
    setup_texts();
}

void SelectState::load_font(){
    m_font = Resources::MainFont();
}

void SelectState::setup_texts(){

    m_title_text.setFont(m_font);
    m_title_text.setCharacterSize(50);
    m_title_text.setString("Gojo Sukuna Fight");
    m_title_text.setFillColor(sf::Color::White);
    m_title_text.setPosition(sf::Vector2f(m_window.getSize().x/2.0f - m_title_text.getGlobalBounds().size.x/2.0f, 100.0f));

    m_start_text.setFont(m_font);
    m_start_text.setCharacterSize(30);
    m_start_text.setString("Start Game");
    m_start_text.setPosition(sf::Vector2f(m_window.getSize().x/2.0f - m_start_text.getGlobalBounds().size.x/2.0f, 250.0f));

    m_exit_text.setFont(m_font);
    m_exit_text.setCharacterSize(30);
    m_exit_text.setString("Exit Game");
    m_exit_text.setPosition(sf::Vector2f(m_window.getSize().x/2.0f - m_exit_text.getGlobalBounds().size.x/2.0f, 300.0f));

}

void SelectState::move_selection_up(){
    if (m_select_option == SelectOption::Exit)
        m_select_option = SelectOption::Start;
}

void SelectState::move_selection_down(){
    if (m_select_option == SelectOption::Start)
        m_select_option = SelectOption::Exit;
}

void SelectState::confirm_selection(){
    switch (m_select_option)
    {
        case SelectOption::Start:
            std::cout << "Starting Game" << std::endl;
            set_next_state(std::make_unique<GameState>(m_state_manager, sf::VideoMode({1250,1250}),"Game Window"));
            break;
        case SelectOption::Exit:
            std::cout << "Exiting Game" << std::endl;
            set_next_state(std::make_unique<ExitState>(m_state_manager));
            break;
    }
}

void SelectState::event_handling(){
    while (auto event = m_window.pollEvent())
    {
        if (event.value().is<sf::Event::Closed>())
            m_window.close();
        if (auto event_resized = event.value().getIf<sf::Event::Resized>()) {
            sf::View view = m_window.getView();
            view.setSize(sf::Vector2f(event_resized->size.x, event_resized->size.y));
            m_window.setView(view);
        }
        if (auto event_key = event.value().getIf<sf::Event::KeyPressed>()) {
            if (event_key->code == sf::Keyboard::Key::Up)
                move_selection_up();
        }
        if (auto event_key = event.value().getIf<sf::Event::KeyPressed>()) {
            if (event_key->code == sf::Keyboard::Key::Down)
                move_selection_down();
        }
        if (auto event_key = event.value().getIf<sf::Event::KeyPressed>()) {
            if (event_key->code == sf::Keyboard::Key::Space)
                confirm_selection();
        }
    }
}

void SelectState::update(){
    if (m_select_option == SelectOption::Start){
        m_start_text.setFillColor(sf::Color::Green);
        m_exit_text.setFillColor(sf::Color::White);
    }
    else{
        m_start_text.setFillColor(sf::Color::White);
        m_exit_text.setFillColor(sf::Color::Green);
    }
}

void SelectState::render(){
    m_window.clear(sf::Color::Black);
    m_window.draw(m_title_text);
    m_window.draw(m_start_text);
    m_window.draw(m_exit_text);
    m_window.display();
}

bool SelectState::do_step(){
    if(!m_window.isOpen())
        return false;
    event_handling();
    update();
    render();
    return true;
}