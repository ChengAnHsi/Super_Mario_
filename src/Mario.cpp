//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include <chrono>

void Mario::jump() {
    if (!isJumping) {
        // setting jump base speed
        velocityY = JUMP_STRENGTH;
        isJumping = true;
    }
}

void Mario::update() {
    if (isJumping) {
        float mario_x = GetPosition().x;
        float mario_y = GetPosition().y;

        // update Y axis position with gravity
        mario_y += velocityY;
        // Gravity continuously slows down the character's ascent and speeds up his descent
        velocityY -= GRAVITY;

        // assume y = 0 is floor
        if (mario_y <= -140.5) {
            mario_y = -140.5;
            isJumping = false;
            velocityY = 0;
        }

        SetPosition({mario_x, mario_y});
    }
}

void Mario::move() {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;

    if (is_dead) {
        this->SetImages(this->AnimationDead);
        return ;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        this->SetImages(this->AnimationJump);
        jump();
        update();
        /**fl_previous_time = fl_current_time;
        now = std::chrono::system_clock::now();
        duration = now.time_since_epoch();
        fl_current_time = std::chrono::duration<double>(duration).count();

        float dt = fl_current_time - fl_previous_time;
        if (dt > 0.15f) {
            dt = 0.15f;
        }
        update(dt);**/
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        this->SetLooping(false);
        this->SetImages(this->AnimationGrow);
        if (this->IfAnimationEnds()) {
            this->SetLooping(true);
            this->SetImages(this->AnimationStandGrow);
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        this->SetPosition({mario_x - 5.0f, mario_y});
        this->SetImages(this->AnimationRun);
        this->SetPlaying(true);
        this->SetLooping(true);
        if (facing == 'l') {

        }else {

        }
        facing = 'l';
    }else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        this->SetPosition({mario_x + 5.0f, mario_y});
        this->SetImages(this->AnimationRun);
        this->SetPlaying(true);
        this->SetLooping(true);
        if (facing == 'l') {

        }else {

        }
        facing = 'r';
    }else {
        this->SetImages(this->AnimationStand);
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