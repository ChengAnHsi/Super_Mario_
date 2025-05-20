//
// Created by 112590007 on 2025/5/20.
//
#include "BackgroundImage.hpp"
#include "Blocks/Block.hpp"

class Fireball : public BackgroundImage {
public:
    Fireball(const glm::vec2& pos, bool right) {
        SetPosition(pos.x, pos.y);
        m_Velocity = right ? glm::vec2(5.0f, 0.0f) : glm::vec2(-5.0f, 0.0f);
    }

    void Update(); // update position and
    void CheckCollisionWithBlocks(const std::vector<std::shared_ptr<Block>>& blocks);
    void CheckCollisionWithBoxes(const std::vector<std::shared_ptr<BackgroundImage>>& blocks);

private:
    glm::vec2 m_Velocity;
};
