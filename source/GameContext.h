#pragma once
#include <stack>
#include "Maze.h"

class IStaticEntity;
class IDynamicEntity;
class Food;
class IGameEvent;
class GameContext;
class Enemy;
class ContextManager;

class IVisitor : virtual public IEntity
{
public:
    IVisitor() = default;
    virtual ~IVisitor() = default;
    virtual std::unique_ptr<IGameEvent> visit(Food* ptr_food) = 0;
    virtual std::unique_ptr<IGameEvent> visit(Enemy* ptr_enemy) = 0;
};
class IVisitable : virtual public IEntity
{
public:
    IVisitable() = default;
    virtual ~IVisitable() override = default;
    virtual std::unique_ptr<IGameEvent> accept(IVisitor* ptr_visitor) = 0;
};

class Pacman : public IVisitor{
private:
    sf::Sprite m_sprite;
    int m_step_count_left = 0;
    int m_step_count_right = 0;
public:
    Pacman();
    ~Pacman() = default;
    Pacman(const Pacman& other) = default;
    void move(Direction direction);
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
    std::unique_ptr<IGameEvent> visit(Food* ptr_food) override;
    std::unique_ptr<IGameEvent> visit(Enemy* ptr_enemy) override;
};

class IStaticEntity : public IVisitable{
public:
    virtual ~IStaticEntity() override = default;
    virtual std::unique_ptr<IStaticEntity> clone() const = 0;
    virtual void set_eaten(bool) = 0;
    virtual bool is_eaten() const = 0;
};

class IDynamicEntity : public IVisitable{
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
    sf::Sprite m_sprite;
    float m_move_accumulator = 0.0f;
    float m_move_interval = 0.7f;
public:
    Enemy();
    Enemy(const Enemy& other) = default;
    std::unique_ptr<IDynamicEntity> clone() const override{return std::make_unique<Enemy>(*this);}
    void action() override;
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
    std::unique_ptr<IGameEvent> accept(IVisitor* ptr_visitor) override;
};

class Food : public IStaticEntity{
private:
    bool m_is_eaten = false;
    sf::Sprite m_sprite;
public:
    Food();
    Food(const Food& other) = default;
    std::unique_ptr<IStaticEntity> clone() const override{return std::make_unique<Food>(*this);}
    bool is_eaten() const {return m_is_eaten;}
    void set_eaten(bool eaten) override {m_is_eaten = eaten;}
    void packman_eaten() {m_is_eaten = true;}
    void prepare_for_drawing() override;
    void draw_into(sf::RenderWindow& window) const override;
    std::unique_ptr<IGameEvent> accept(IVisitor* ptr_visitor) override;
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

class ContextManager{
private:
    GameContext m_initial_context;
    std::stack<GameContext> m_contexts;
public:
    ContextManager() = default;
    ~ContextManager() = default;
    void reset(GameContext&& initial_context);
    GameContext& get_current_context() {return m_initial_context;}
    void save_current_context(){ m_contexts.push(m_initial_context);}
    void restore_previous_context();
};