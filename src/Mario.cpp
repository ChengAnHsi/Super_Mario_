//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <chrono>

/*void Mario::update(float dt) {
    float mario_x =  this->GetPosition().x + velocity[0] * dt;
    float mario_y =  this->GetPosition().y + velocity[1] * dt;
    velocity[0] = velocity[0] + gravity[0] * dt;
    velocity[1] = velocity[1] + gravity[1] * dt;
    SetPosition({mario_x,mario_y});
}*/

void Mario::move() {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;

    /**float fl_previous_time = 0;
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    float fl_current_time = std::chrono::duration<float>(duration).count();**/


    if (is_dead) {
        this->SetImages(this->AnimationDead);
        return ;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        this->SetPosition({mario_x, mario_y + 5.0f});
        /**fl_previous_time = fl_current_time;
        now = std::chrono::system_clock::now();
        duration = now.time_since_epoch();
        fl_current_time = std::chrono::duration<double>(duration).count();

        float dt = fl_current_time - fl_previous_time;
        if (dt > 0.15f) {
            dt = 0.15f;
        }
        update(dt);**/
        this->SetImages(this->AnimationJump);
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        this->SetPosition({mario_x - 5.0f, mario_y});
        this->SetImages(this->AnimationRun);
        if (facing == 'l') {
        }else {
        }
        facing = 'l';
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        this->SetPosition({mario_x + 5.0f, mario_y});
        this->SetImages(this->AnimationRun);
        if (facing == 'l') {
        }else {
        }
        facing = 'r';
    }
}


void Mario::Increase_Coin(const int coin) {
    this->coin += coin;
}

int Mario::Get_Coin() {
    return coin;
}

void Mario::Set_Live(const int live) {
    this->live = live;
}

int Mario::Get_Live() {
    return coin;
}

void Mario::Increase_Score(const int score) {
    this->score += score;
}

int Mario::Get_Score() {
    return coin;
}