//
// Created by 112590007 on 2025/4/25.
//

#ifndef FLOWER_HPP
#define FLOWER_HPP

#include "Enemy.hpp"
#include "CollisionState.hpp"
#include "Mario.hpp"
class Flower : public Enemy {
public:
    Flower();


    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 flower_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;
    bool CheckMarioCollision(std::shared_ptr<Mario> mario) override;
    void UpdateAnimation() override;

    // Re-calculate Y movement range based on current position
    void UpdateYMovementRange();

    // Set position with range update
    virtual void  SetPosition(float x, float y);

    // getter and setter
    void SetLive(int live) override;
    [[nodiscard]] int GetLive() const;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;

private:
    bool isFacingUp = true;

    // Y-axis movement boundaries (will be calculated in constructor and SetPosition)
    float min_y_position = 0.0f;
    float max_y_position = 0.0f;

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Underworld/flower0.png",RESOURCE_DIR"/Entities/Underworld/flower1.png"};

    // bool is_dead = false;
    bool is_set_runanimation = false;
};
#endif //FLOWER_HPP