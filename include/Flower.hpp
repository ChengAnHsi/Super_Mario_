//
// Created by 112590007 on 2025/3/6.
//

#ifndef FLOWER_HPP
#define FLOWER_HPP

#include "Enemy.hpp"
#include "Block.hpp"
#include "CollisionState.hpp"

class Flower : public Enemy{
public:
    Flower() = default;

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;

    void UpdateAnimation() override;

    // Set the Y-axis movement range
    void SetYMovementRange(float min_y, float max_y);

    // getter and setter
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;

private:
    int live = 1;
    // 被擊倒的分數
    int score = 100;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isFacingUp = true;
    float delta_time = 1.0f;

    // Y-axis movement boundaries
    float min_y_position = GetPosition().y-m_Drawable->GetSize().y;  // Default minimum Y position
    float max_y_position = GetPosition().y;   // Default maximum Y position

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Underworld/flower0.png",RESOURCE_DIR"/Entities/Underworld/flower1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/Overworld/goombaDead.png"};

    bool is_dead = false;
    bool is_set_runanimation = false;
};
#endif //FLOWER_HPP