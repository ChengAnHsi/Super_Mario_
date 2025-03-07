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
    //void update(float dt);
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

    std::vector<float> velocity = {2, 2};
    std::vector<float> gravity = {0, -2};

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/mario0.png",RESOURCE_DIR"/Entities/mario1.png",RESOURCE_DIR"/Entities/mario2.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/marioDead.png"};
    std::vector<std::string> AnimationJump = {RESOURCE_DIR"/Entities/mario_Jump.png"};
    std::vector<std::string> AnimationWalk = {RESOURCE_DIR"/Entities/marioWalk.png"};
    bool is_dead = false;
};
#endif //MARIO_HPP
