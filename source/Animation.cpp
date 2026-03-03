#include "Animation.h"

void Animation::update(float deltaTime){
    if (!m_is_playing || m_is_finished || m_frames.empty()) return;
    m_current_time += deltaTime;
    while (m_current_time>=m_frame_duration){
        m_current_time -= m_frame_duration;
        switch (m_mode){
            case Mode::Loop:
                m_current_frame = (m_current_frame+1) % m_frames.size();
                break;
            case Mode::PingPong:
                m_current_frame += m_direction;
                if (m_current_frame >= m_frames.size() - 1 || m_current_frame==0)
                {
                    m_direction *= -1;
                    if (!m_loops_back && (m_current_frame == 0) )
                        m_is_finished = true;
                }
                break;
            case Mode::OneShot:
                if (m_current_frame < m_frames.size() - 1)
                    m_current_frame ++;
                else{
                    m_is_finished = true;
                    m_is_playing = false;
                }
                break;
            case Mode::Sequence:
                if (m_current_frame < m_frames.size() - 1)
                    m_current_frame ++;
                break;
        }
    }
}

const sf::Texture* Animation::get_current_frame() const{
    if (m_frames.empty()) return nullptr;
    return m_frames[m_current_frame];
}