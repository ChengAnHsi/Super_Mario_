#include "AnimatedCharacter.hpp"

AnimatedCharacter::AnimatedCharacter(const std::vector<std::string>& AnimationPaths) {
    m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, true, 50, false, 0);
}

bool AnimatedCharacter::IfAnimationEnds() const {
    auto animation = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        return animation->GetState() == Util::Animation::State::ENDED || animation->GetState() == Util::Animation::State::PAUSE;
}
