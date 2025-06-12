#ifndef ANIMATED_CHARACTER_HPP
#define ANIMATED_CHARACTER_HPP

#include <vector>
#include <string>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class AnimatedCharacter : public Util::GameObject {

public:
    explicit AnimatedCharacter(const std::vector<std::string>& AnimationPaths);
    ~AnimatedCharacter() override = default;

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    [[nodiscard]] glm::vec2 GetSize() const {
        return m_Drawable->GetSize();
    }

    void SetImages(const std::vector<std::string>& AnimationPaths, int interval, int cooldown) {
        m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, IsPlaying(), interval, IsLooping(), cooldown);
    }

    void SetLooping(const bool looping) const {
        const auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }

    void SetPlaying(const bool playing) const {
        const auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if(playing) {
            temp->Play();
        }else {
            temp->Pause();
        }
    }

    void SetPosition(float posX, float posY) {
        m_Transform.translation.x = posX;
        m_Transform.translation.y = posY;
    }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool IfAnimationEnds() const;

};

#endif //ANIMATED_CHARACTER_HPP
