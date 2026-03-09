#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class IStateManager;

class IState{
public:
    explicit IState(IStateManager& state_manager) : m_state_manager(state_manager) {}
    virtual ~IState() = default;
    virtual bool do_step() = 0;
protected:
    void set_next_state(std::unique_ptr<IState> next_state);
    IStateManager& m_state_manager;
};

class IStateManager{
public:
    virtual ~IStateManager() = default;
    virtual void set_next_state(std::unique_ptr<IState> state) = 0;
};

class Application : public IStateManager
{
private:
    std::unique_ptr<IState> m_ptr_state_next;
    std::unique_ptr<IState> m_ptr_state_current;
private:
    void set_next_state(std::unique_ptr<IState> state) override;
    void apply_deffer_state_change();
public:
    Application() = default;
    ~Application() override = default;

    void set_initial_state(std::unique_ptr<IState> initial_state);
    int run();
};

class IWindowKeeper
{
protected:
    sf::RenderWindow m_window;
public:
    IWindowKeeper(sf::VideoMode video_mode, const std::string& window_title)
        : m_window(video_mode, window_title, sf::Style::Default){}
    virtual ~IWindowKeeper() = default;
    bool is_window_open() const {return m_window.isOpen();}
    void close_window() {m_window.close();}
protected:
    virtual void event_handling() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

inline void IState::set_next_state(std::unique_ptr<IState> next_state){
    m_state_manager.set_next_state(std::move(next_state));
}