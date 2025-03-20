//
// Created by 112590007 on 2025/3/6.
//

#ifndef MARIO_HPP
#define MARIO_HPP
#include "AnimatedCharacter.hpp"
#include <iostream>
#include <string>

#include "AnimatedCharacter.hpp"
#include "Util/Animation.hpp"

class Mario : public AnimatedCharacter {
public:
    Mario(int coin, int live, int score, const std::vector<std::string>& AnimationPaths): AnimatedCharacter(AnimationPaths){
        this->coin = coin;
        this->live = live;
        this->score = score;
    };
    void on_jump();
    void on_run(float distance);
    void move_and_collision(int delta);
    float on_update(int delta);
    float move();
    void Increase_Coin(int coin);
    [[nodiscard]] int Get_Coin() const;
    void Set_Live(int live);
    [[nodiscard]] int Get_Live() const;
    void Increase_Score(int score);
    [[nodiscard]] int Get_Score() const;
    bool is_on_floor() const;
    bool has_block_underneath() const;
    void calculate_falling_speed();
    void on_smalljump();
private:
    int coin = 0;
    int live = 3;
    int score = 0;
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_facing_right = true;

    bool isJumping = false;
    bool isRunning = false;
    float temp= 0.0f;      //暫存
    float delta_time = 1.0f;
    float run_velocity = 5.0f;
    float jump_velocity = 12.7f;
    float velocityY = 0.0f;      // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    float JUMP_VELOCITY = 350.0f; // 跳躍初速度
    float SMALL_JUMP_VELOCITY = 250.0f; // 小跳躍初速度
    float MAX_JUMP_VELOCITY = 50.0f; // 最高跳躍值
    float additional_jump_forc0 = 0.1f; // 玩家按下按鍵時間，跳躍提升值
    const float FIXED_TIME_STEP = 1.0f / 60.0f;  // 固定更新步長，假設 60 FPS
    const float FIXED_JUMP_SPEED = 8.0f;  // Mario 每幀上升的固定速度
    const float FIXED_GRAVITY = -0.9f;  // Mario 每幀下降的固定速度

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
