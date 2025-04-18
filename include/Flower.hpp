// Modified Flower.hpp
#ifndef FLOWER_HPP
#define FLOWER_HPP

#include "Enemy.hpp"
#include "Block.hpp"
#include "CollisionState.hpp"

class Flower : public Enemy {
public:
    Flower();  // Changed to non-default constructor to initialize Y range

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;

    void UpdateAnimation() override;

    // Re-calculate Y movement range based on current position
    void UpdateYMovementRange();

    // Set position with range update
    virtual void  SetPosition(float x, float y);

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

    // Y-axis movement boundaries (will be calculated in constructor and SetPosition)
    float min_y_position = 0.0f;
    float max_y_position = 0.0f;

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Underworld/flower0.png",RESOURCE_DIR"/Entities/Underworld/flower1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/Overworld/goombaDead.png"};

    bool is_dead = false;
    bool is_set_runanimation = false;
};
#endif //FLOWER_HPP