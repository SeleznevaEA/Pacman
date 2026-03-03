#pragma once
#include "Maze.h"
#include "TextureCache.h"
#include "Animation.h"

class Pacman : public IEntity{
private:
    sf::Sprite m_sprite;
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_current_animation;
    Direction m_direction;
    bool is_moving;
public:
    Pacman();
    ~Pacman();
    Pacman(const Pacman& other);
    void move(Direction direction);
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
};

class IStaticEntity : public IEntity{
public:
    virtual ~IStaticEntity() override = default;
    virtual std::unique_ptr<IStaticEntity> clone() const = 0;
};

class IDynamicEntity : public IEntity{
protected:
    sf::Clock m_action_clock;
public:
    virtual ~IDynamicEntity() override = default;
    virtual std::unique_ptr<IDynamicEntity> clone() const =0;
    virtual void action() =0;
};

class Enemy : public IDynamicEntity{
private:
    sf::Clock m_stopwatch;
public:
    Enemy() = default;
    Enemy(const Enemy& other) = default;
    std::unique_ptr<IDynamicEntity> clone() const override{return std::make_unique<Enemy>(*this);}
    void action() override;
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
};

class Food : public IStaticEntity{
private:
    bool m_is_eaten = false;
public:
    Food() = default;
    Food(const Food& other) = default;
    std::unique_ptr<IStaticEntity> clone() const override{return std::make_unique<Food>(*this);}
    bool is_eaten() const {return m_is_eaten;}
    void set_eaten(bool eaten) {m_is_eaten = eaten;}
    void packman_eaten() {m_is_eaten = true;}
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
};

class GameContext{
public:
    enum class State{
        INGAME, WIN, LOST
    };
    std::unique_ptr<Pacman> pacman;
    std::vector<std::unique_ptr<IStaticEntity>> static_objects;
    std::vector<std::unique_ptr<IDynamicEntity>> dynamic_objects;
    State state = State::INGAME;

    TextureCache texture_cache;
public:
    GameContext() = default;
    ~GameContext() = default;
    GameContext(const GameContext& other);
    GameContext& operator=(const GameContext& other);
    GameContext clone() const {return GameContext(*this);}
    Pacman* get_pacman() const {return pacman.get();}
    void set_pacman(std::unique_ptr<Pacman> pac) {pacman = std::move(pac);}
    void add_static_object(std::unique_ptr<IStaticEntity> object){static_objects.push_back(std::move(object));}
    void add_dynamic_object(std::unique_ptr<IDynamicEntity> object){dynamic_objects.push_back(std::move(object));}
    const std::vector<std::unique_ptr<IStaticEntity>>& get_static_objects() const {return static_objects;}
    const std::vector<std::unique_ptr<IDynamicEntity>>& get_dynamic_objects() const {return dynamic_objects;}
    State get_state() const {return state;}
    void set_state(State s) {state = s;}
    void clear();
    void update();
    void check_game_state();
};
