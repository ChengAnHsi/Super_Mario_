#ifndef ANIMATED_CHARACTER_HPP
#define ANIMATED_CHARACTER_HPP

#include <vector>
#include <string>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"


class AnimatedCharacter : public Util::GameObject {

public:
    explicit AnimatedCharacter(const std::vector<std::string>& AnimationPaths);

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetImages(const std::vector<std::string>& AnimationPaths) {
        m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, IsPlaying(), 500, IsLooping(), 0);
    }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }

    void SetPlaying(bool playing) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        if(playing) {
            temp->Play();
        }else {
            temp->Pause();
        }
    }

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool IfAnimationEnds() const;

};

#endif //ANIMATED_CHARACTER_HPP
