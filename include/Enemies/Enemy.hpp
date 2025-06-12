//
// Created by 112590007 on 2025/4/4.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AnimatedImage.hpp"
#include "Blocks/Block.hpp"
#include "DeadState.hpp"
#include "Mario.hpp"

class Enemy : public BackgroundImage {

public:
    Enemy() = default;
    ~Enemy() override = default;

    virtual void Move() = 0;
    virtual void Action(float distance) = 0;
    virtual bool GravityAndCollision(float delta) = 0;
    virtual void UpdateAnimation() = 0;
    //void OnUpdate(float delta);

    // getter and setter
    virtual void SetLive(int live) = 0;
    int GetLive();
    [[nodiscard]] bool GetMoving() const {return isMoving;}
    void SetMoving(const bool moving) {isMoving = moving;}
    float GetMoveVelocity();
    bool GetOverworld();
    bool GetIsDead();
    void SetDeadState(DeadState deadState);
    void SetOverworld(bool is_overworld);
    void SetVelocityX(float velocityX);
    void SetVelocityY(float velocityY);
    bool GetFacingRight();
    void SetFacingRight(bool facingRight);
    int GetScore();

    virtual void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) = 0;
    virtual void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) = 0;
    virtual void ClearCollisionBoxes() = 0;
    virtual void ClearCollisionBlocks() = 0;
    virtual bool CheckMarioCollision(std::shared_ptr<Mario> mario) = 0;
    bool CheckFireballCollision(const std::shared_ptr<Fireball>& fireball);
protected:
    int live = 1;
    int score = 100; // 被擊倒的分數
    bool isFacingRight = false;

    float velocityY = 0.0f; // 角色在 Y 軸的速度
    float GRAVITY = -300.0f;

    float delta_time = 1.0f;

    // dead
    bool is_dead = false;
    float death_timer = 0.0f;
    const float DEATH_JUMP_VELOCITY = 300.0f;
    const float DEATH_JUMP_DURATION = 120.0f;
    const float DEATH_ANIMATION_TIME = 80.0f;
    DeadState dead_state = DeadState::Alive;

    // collision
    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;
private:
    bool isMoving = false;
    float move_velocity = 2.0f;
    bool isOverWorld = true;
};
#endif //ENEMY_HPP
