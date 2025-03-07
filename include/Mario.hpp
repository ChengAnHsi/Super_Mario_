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
    void move();
    void jump();
    void update();
    void Increase_Coin(int coin);
    int Get_Coin();
    void Set_Live(int live);
    int Get_Live();
    void Increase_Score(int score);
    int Get_Score();
private:
    int coin = 0;
    int live = 3;
    int score = 0;
    char facing = 'r';

    float velocityY = 0;      // 角色在 Y 軸的速度
    bool isJumping = false;

    float GRAVITY = 9.8f;  // 重力加速度
    float JUMP_STRENGTH = 30.0f;  // 跳躍的初速度


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
