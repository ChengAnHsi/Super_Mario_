//
// Created by 112590007 on 2025/5/20.
//

#include "Props\Fireball.hpp"

void Fireball::Update() {
    glm::vec2 pos = GetPosition();
    pos += m_Velocity;
    SetPosition(pos.x, pos.y);
}

void Fireball::CheckCollisionWithBlocks(const std::vector<std::shared_ptr<Block>>& blocks) {
    // todo
}

void Fireball::CheckCollisionWithBoxes(const std::vector<std::shared_ptr<BackgroundImage>>& blocks) {
    // todo
}
