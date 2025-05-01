//
// Created by 112590007 on 2025/3/6.
//

#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Enemy.hpp"
#include "../Blocks/Block.hpp"
#include "../CollisionState.hpp"
#include "../Mario.hpp"

class Goomba : public Enemy{
public:
    Goomba() = default;

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;

    void UpdateAnimation() override;

    // getter and setter
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;
    bool CheckMarioCollision(std::shared_ptr<Mario> mario);
    void KillGoomba();
    bool IsDead() const { return is_dead; }
    // TODO 被擊倒動畫
    bool is_dead = false;
private:
    int live = 1;
    // 被擊倒的分數
    int score = 100;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isFacingRight = false;
    float delta_time = 1.0f;
    float velocityY = 0.0f; // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位

    // TODO: dead update
    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Overworld/goomba0.png",RESOURCE_DIR"/Entities/Overworld/goomba1.png"};
    std::vector<std::string> AnimationUnderWorldRun = {RESOURCE_DIR"/Entities/Underworld/goomba0.png",RESOURCE_DIR"/Entities/Underworld/goomba1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/Overworld/goombaDead.png"};
    std::vector<std::string> AnimationUnderWorldDead = {RESOURCE_DIR"/Entities/Underworld/goombaDead.png"};


    float death_timer = 0.0f;
    const float DEATH_ANIMATION_TIME = 80.0f;
    bool is_set_runanimation = false;
};
#endif //GOOMBA_HPP

