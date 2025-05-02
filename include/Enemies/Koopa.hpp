//
// Created by 112590007 on 2025/3/6.
//

#ifndef KOOPA_HPP
#define KOOPA_HPP

#include "Enemy.hpp"
#include "Blocks/Block.hpp"
#include "CollisionState.hpp"
#include "Mario.hpp"
class Koopa : public Enemy{
public:
    Koopa() = default;

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
    // TODO 被擊倒動畫
    void TurnToShell();
    bool KickShell(std::shared_ptr<Mario> mario);
private:

    int live = 1;
    // 被擊倒的分數
    int score = 100;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
    float shell_timer = 0.0f;
    bool isFacingRight = false;
    float delta_time = 1.0f;
    float velocityY = 0.0f; // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    bool is_shell = false;
    bool shell_is_moving = false;
    // TODO: dead update
    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/koopa0.png",RESOURCE_DIR"/Entities/koopa1.png"};
    std::vector<std::string> AnimationUnderWorldRun = {RESOURCE_DIR"/Entities/Underworld/koopa0.png",RESOURCE_DIR"/Entities/Underworld/koopa1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/shell1.png"};
    std::vector<std::string> AnimationUnderWorldDead = {RESOURCE_DIR"/Entities/Underworld/shell1.png"};

    bool is_dead = false;
    bool is_set_runanimation = false;
};
#endif //KOOPA_HPP

