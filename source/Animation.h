#pragma once
#include <vector>

#include "TextureCache.h"

class Animation
{
public:
    enum class Mode
    {
        Loop, PingPong, OneShot, Sequence
    };
private:
    std::vector<const sf::Texture*> m_frames;
    Mode m_mode = Mode::Sequence;
    float m_frame_duration;
    float m_current_time;
    size_t m_current_frame;
    int m_direction; // 1 или -1 для PingPong
    bool m_is_playing;
    bool m_loops_back; // тоже для пинпонга
    bool m_is_finished;

public:
    explicit Animation(Mode mode = Mode::Loop, float frame_duration = 0.1f):
        m_mode(mode), m_frame_duration(frame_duration), m_is_playing(true), m_loops_back(false),
        m_current_frame(0), m_current_time(0), m_direction(1), m_is_finished(false){}
    void add_frame(const sf::Texture& texture){m_frames.push_back(&texture);}
    void update(float deltaTime);
    void reset(){ m_current_time = 0; m_current_frame = 0; m_is_playing = true; m_direction = 1; m_is_finished = false; }
    const sf::Texture* get_current_frame() const;
    void set_mode(Mode mode){m_mode = mode;}
    void set_frame_duration(float frame_duration){m_frame_duration = frame_duration;}
    void set_loops_back(bool loops_back){m_loops_back = loops_back;}
    bool is_finished() const { return m_is_finished; }
    bool is_playing() const { return m_is_playing; }
    void play() {m_is_playing = true;}
    void stop() {m_is_finished = false;}
    void pause() {m_is_playing = false;}
};