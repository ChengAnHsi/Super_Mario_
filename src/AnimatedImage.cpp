//
// Created by 112590007 on 2025/4/4.
//

#include "AnimatedImage.hpp"
#include "Util/Animation.hpp"

void AnimatedImage::SetLooping(bool looping) const {
    const auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    temp->SetLooping(looping);
}

void AnimatedImage::SetPlaying(bool playing) const {
    const auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    if (playing) {
        temp->Play();
    } else {
        temp->Pause();
    }
}

bool AnimatedImage::IfAnimationEnds() const {
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
    return animation->GetCurrentFrameIndex() == animation->GetFrameCount() - 1;
}
