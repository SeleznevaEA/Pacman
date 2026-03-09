#include "Builders.h"
//GameBuilderDirector

GameBuilderDirector::GameBuilderDirector(IGameBuilder* ptr_builder,
    sf::VideoMode video_mode, std::string window_title, float dynamic_object_ratio)
    : m_ptr_builder(ptr_builder), m_window_title(std::move(window_title)),
    m_dynamic_object_ratio(dynamic_object_ratio), m_mode(video_mode)
{}

GameBuilderDirector::~GameBuilderDirector() {
    delete m_ptr_builder;
}

GameState* GameBuilderDirector::build(IStateManager& state_manager){
    if (!m_ptr_builder) return nullptr;

    m_ptr_builder -> create_rooms();
    m_ptr_builder -> set_room_sides();
    m_ptr_builder -> create_context(m_dynamic_object_ratio);
    m_ptr_builder -> create_state(state_manager, m_mode, m_window_title);
    m_ptr_builder -> set_all_to_state();
    return m_ptr_builder ->get_game();
}


//CommonBuilder

CommonBuilder::CommonBuilder(sf::VideoMode video_mode, std::string window_title)
    : m_width(video_mode.size.x), m_height(video_mode.size.y),
    m_room_size(50.0f), m_context(nullptr), m_game_state(nullptr)
{
    int rows = static_cast<int>(m_height/m_room_size);
    int cols = static_cast<int>(m_width/m_room_size);
    m_rooms.resize(rows, std::vector<Room*> (cols, nullptr));
}

CommonBuilder::~CommonBuilder(){
    delete_rooms();
    delete m_context;
    delete m_game_state;
}

bool CommonBuilder::has_room(int row, int col) const{
    if (row < 0 || row >= static_cast<int>(m_rooms.size()) ||
        col < 0 || col >= static_cast<int>(m_rooms[0].size())) {
        return false;
        }
    return m_rooms[row][col] != nullptr;
}

void CommonBuilder::create_room_at(int row, int col){
    if (row < 0 || row >= static_cast<int>(m_rooms.size()) ||
        col < 0 || col >= static_cast<int>(m_rooms[0].size())) {
        return;
        }

    if (!m_rooms[row][col]) {
        m_rooms[row][col] = new Room(m_room_size);

        sf::Vector2f position(col * m_room_size, row * m_room_size);
        m_rooms[row][col]->set_position(position);
    }
}

void CommonBuilder::delete_rooms() {
    for (auto& row : m_rooms) {
        for (auto* room : row) {
            delete room;
        }
    }
    m_rooms.clear();
}

void CommonBuilder::create_context(float dynamic_object_ratio) {
    m_context = new GameContext();
    // Здесь будет логика размещения объектов и персонажей
    // Пока создаем пустой контекст
}

void CommonBuilder::create_state(IStateManager& state_manager, sf::VideoMode& video_mode,
    std::string window_title) {
    m_game_state = new GameState(state_manager, video_mode, window_title);
}


void CommonBuilder::set_all_to_state(){
    if (!m_game_state) return;

    std::vector<Room*> rooms;
    for (auto& row : m_rooms) {
        for (auto* room : row) {
            if (room) {
                rooms.push_back(room);
            }
        }
    }

    Maze maze(rooms);
    m_game_state->set_maze(std::move(maze));
    
    // Обнуляем указатели на комнаты - владение передано Maze
    for (auto& row : m_rooms) {
        for (auto*& room : row) {
            room = nullptr;
        }
    }

    if (m_context) {
        m_game_state->set_context(std::move(*m_context));
    }
}

GameState* CommonBuilder::get_game() {
    GameState* temp = m_game_state;
    m_game_state = nullptr;  // Передаём владение вызывающей стороне
    return temp;
}


//SimpleBuilder

SimpleBuilder::SimpleBuilder(sf::VideoMode video_mode, std::string window_title)
    : CommonBuilder(video_mode, std::move(window_title))
{}

void SimpleBuilder::create_rooms() {
    for (int row = 0; row < static_cast<int>(m_rooms.size()); ++row) {
        for (int col = 0; col < static_cast<int>(m_rooms[0].size()); ++col) {
            create_room_at(row, col);
        }
    }
}

void SimpleBuilder::set_room_sides() {
    int rows = static_cast<int>(m_rooms.size());
    int cols = static_cast<int>(m_rooms[0].size());

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Room* current = m_rooms[row][col];
            if (!current) continue;

            // UP
            if (row > 0 && m_rooms[row-1][col]) {
                current->set_side(UP, new Pass(*current, *m_rooms[row-1][col]));
            } else {
                current->set_side(UP, new Wall(*current));
            }

            // DOWN
            if (row < rows-1 && m_rooms[row+1][col]) {
                current->set_side(DOWN, new Pass(*current, *m_rooms[row+1][col]));
            } else {
                current->set_side(DOWN, new Wall(*current));
            }

            // LEFT
            if (col > 0 && m_rooms[row][col-1]) {
                current->set_side(LEFT, new Pass(*current, *m_rooms[row][col-1]));
            } else {
                current->set_side(LEFT, new Wall(*current));
            }

            // RIGHT
            if (col < cols-1 && m_rooms[row][col+1]) {
                current->set_side(RIGHT, new Pass(*current, *m_rooms[row][col+1]));
            } else {
                current->set_side(RIGHT, new Wall(*current));
            }
        }
    }
}



//ComplexBuilder

ComplexBuilder::ComplexBuilder(sf::VideoMode video_mode, std::string window_title)
    : CommonBuilder(video_mode, std::move(window_title))
{}

void ComplexBuilder::create_rooms() {
    int rows = static_cast<int>(m_rooms.size());
    int cols = static_cast<int>(m_rooms[0].size());

    for (int row = 1; row < rows-1; ++row) {
        for (int col = 1; col < cols-1; ++col) {
            if (rand() % 3 != 0) {
                create_room_at(row, col);
            }
        }
    }

    // Создаем граничные комнаты для связности
    for (int col = 0; col < cols; ++col) {
        create_room_at(0, col);        // верхняя граница
        create_room_at(rows-1, col);   // нижняя граница
    }

    for (int row = 0; row < rows; ++row) {
        create_room_at(row, 0);        // левая граница
        create_room_at(row, cols-1);   // правая граница
    }
}

void ComplexBuilder::set_room_sides() {
    int rows = static_cast<int>(m_rooms.size());
    int cols = static_cast<int>(m_rooms[0].size());

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            Room* current = m_rooms[row][col];
            if (!current) continue;  // это дырка

            // UP
            if (row > 0 && m_rooms[row-1][col]) {
                current->set_side(UP, new Pass(*current, *m_rooms[row-1][col]));
            } else {
                current->set_side(UP, new Wall(*current));
            }

            // DOWN
            if (row < rows-1 && m_rooms[row+1][col]) {
                current->set_side(DOWN, new Pass(*current, *m_rooms[row+1][col]));
            } else {
                current->set_side(DOWN, new Wall(*current));
            }

            // LEFT
            if (col > 0 && m_rooms[row][col-1]) {
                current->set_side(LEFT, new Pass(*current, *m_rooms[row][col-1]));
            } else {
                current->set_side(LEFT, new Wall(*current));
            }

            // RIGHT
            if (col < cols-1 && m_rooms[row][col+1]) {
                current->set_side(RIGHT, new Pass(*current, *m_rooms[row][col+1]));
            } else {
                current->set_side(RIGHT, new Wall(*current));
            }
        }
    }
}