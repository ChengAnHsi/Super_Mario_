//
// Created by 112590007 on 2025/3/6.
//

#ifndef MARIO_HPP
#define MARIO_HPP
#include "AnimatedCharacter.hpp"
#include "BlockManager.hpp"
#include <iostream>
#include <string>

enum class MarioState {
    Stand,
    Run,
    Jump
};

enum class CollisionState {
    Left,
    Right,
    Top,
    Bottom,
    None
};

class Mario : public AnimatedCharacter {
public:
    Mario(int coin, int live, int score, const std::vector<std::string>& AnimationPaths): AnimatedCharacter(AnimationPaths){
        this->coin = coin;
        this->live = live;
        this->score = score;
    };

    // move function
    void OnJump();
    void OnSmallJump();
    void OnRun(float distance, std::shared_ptr<BlockManager> m_BM);
    float Move(std::shared_ptr<BlockManager> m_BM);
    float OnUpdate(float delta, std::shared_ptr<BlockManager> m_BM);

    // collision function
    bool AABBCollides(glm::vec2 a, glm::vec2 b);
    bool CCDDCollides(glm::vec2 a, glm::vec2 b);
    bool GravityAndCollision(float delta, std::shared_ptr<BlockManager> m_BM);

    void UpdateAnimation();

    // getter and setter
    void IncreaseCoin(int coin);
    [[nodiscard]] int GetCoin() const;
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void IncreaseScore(int score);
    [[nodiscard]] int GetScore() const;

private:
    int coin = 0;
    int live = 3;
    int score = 0;
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_facing_right = true;

    MarioState state = MarioState::Stand;
    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isJumping = false;
    bool isRunning = false;
    float delta_time = 1.0f;
    float run_velocity = 5.0f;
    float jump_velocity = 12.7f;
    float velocityY = 0.0f;      // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    float JUMP_VELOCITY = 360.0f; // 跳躍初速度
    float SMALL_JUMP_VELOCITY = 250.0f; // 小跳躍初速度
    float MAX_JUMP_VELOCITY = 55.0f; // 最高跳躍值

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/mario0.png",RESOURCE_DIR"/Entities/mario1.png",RESOURCE_DIR"/Entities/mario2.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/marioDead.png"};
    std::vector<std::string> AnimationJump = {RESOURCE_DIR"/Entities/mario_Jump.png"};
    std::vector<std::string> AnimationStand = {RESOURCE_DIR"/Entities/mario_stand.png"};

    std::vector<std::string> AnimationGrow = {RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_stand.png",
        RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_stand.png", RESOURCE_DIR"/Entities/mario_grown_squat.png",
    RESOURCE_DIR"/Entities/mario_grown_stand.png",RESOURCE_DIR"/Entities/mario_grown_squat.png", RESOURCE_DIR"/Entities/mario_stand.png",
    RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_grown_stand.png"};
    std::vector<std::string> AnimationRunGrow = {RESOURCE_DIR"/Entities/mario_grown0.png",RESOURCE_DIR"/Entities/mario_grown1.png",RESOURCE_DIR"/Entities/mario_grown2.png"};
    std::vector<std::string> AnimationJumpGrow = {RESOURCE_DIR"/Entities/mario_grown_Jump.png"};
    std::vector<std::string> AnimationStandGrow = {RESOURCE_DIR"/Entities/mario_grown_stand.png"};

    bool is_grow = false;
    bool is_dead = false;
};
#endif //MARIO_HPP

