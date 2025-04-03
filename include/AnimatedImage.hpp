//
// Created by 112590007 on 2025/4/4.
//

#ifndef ANIMATEDIMAGE_HPP
#define ANIMATEDIMAGE_HPP

#include <vector>
#include <string>

#include "BackgroundImage.hpp"
#include "Util/Animation.hpp"

class AnimatedImage : public BackgroundImage {

public:
    AnimatedImage() = default;

    AnimatedImage(const std::vector<std::string>& AnimationPaths, int interval, int cooldown) {
        // use different instructor to setting animation
        SetImage(AnimationPaths, interval, cooldown);
    }

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetLooping(bool looping) const;
    void SetPlaying(bool playing) const;
    [[nodiscard]] bool IfAnimationEnds() const;

};
#endif //ANIMATEDIMAGE_HPP
