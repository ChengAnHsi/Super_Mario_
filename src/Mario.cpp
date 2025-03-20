//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::on_jump() {
    if (is_on_floor()) { // 僅在真正站在地面上才能跳躍
        velocityY = JUMP_VELOCITY;  // 設定跳躍初速度
        // 更新跳躍圖片為統一的 mario.Jump.png
        this->SetImages(this->AnimationJump);
    }
}

void Mario::on_smalljump() {
    if (is_on_floor()) { // 僅在地面上才能小跳
        velocityY = SMALL_JUMP_VELOCITY;  // 設定小跳初速度
        // 更新跳躍圖片為統一的 mario.Jump.png
        this->SetImages(this->AnimationJump);
    }
}

void Mario::on_run(float distance) {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;
    mario_x += distance;
    this->SetPosition({mario_x, mario_y});
}

void Mario::move_and_collision(int delta) {
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    // 每幀依據重力更新速度
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    // 計算對應區間的地面高度
    float ground_level = (mario_x >= -150 && mario_x <= -50) ? -240.0f : -270.0f;

    // 如果角色到達或低於地面，則調整位置並重置速度
    if (mario_y <= ground_level) {
        mario_y = ground_level;
        velocityY = 0;
    }

    this->SetPosition({mario_x, mario_y});
}
bool Mario::is_on_floor() const {
    // 檢查 Mario 是否在對應區間的地面上
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;
    float ground_level = (mario_x >= -150 && mario_x <= -50) ? -240.0f : -270.0f;
    return mario_y <= ground_level;
}


float Mario::on_update(int delta) {
    // action: 0(stand) 1(run)
    int direction = is_right_key_down - is_left_key_down;

    // facing update
    if ((is_facing_right && direction == -1) || (!is_facing_right && direction == 1)) {
        // facing left
        m_Transform.scale = glm::vec2{-2, 2};
    }else if ((is_facing_right && direction == 1) || (is_facing_right && direction == 1)) {
        // facing right
        m_Transform.scale = glm::vec2{2, 2};
    }

    if (direction != 0) {
        is_facing_right = direction;
    }else {
        // animation(stand) update
        isRunning = false;
        if (is_facing_right) {
            this->SetImages(this->AnimationStand);
        }else {
            this->SetImages(this->AnimationStand);
        }
    }
    float distance = direction * run_velocity * delta;
    on_run(distance);
    move_and_collision(3*delta);
    return distance;
}
// 檢查是否有區塊在 Mario 下方
bool Mario::has_block_underneath() const {
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    if (mario_x >= -150 && mario_x <= -50) {
        return mario_y <= -240;
    }
    return mario_y <= -270;

}


float Mario::move() {
    if (is_dead) {
        this->SetImages(this->AnimationDead);
        return 0.0f;
    }
    /**float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;

    if (Util::Input::IsKeyPressed(Util::Keycode::UP) and is_on_floor()) {
        // setting jump base speed
        velocityY = JUMP_STRENGTH;
        this->SetImages(this->AnimationJump);
        isJumping = true;

        velocityY += GRAVITY * delta_time;
        mario_y += velocityY * delta_time;
    }

    if (velocityY > 0) {
        if (velocityY >= MAX_JUMP_VELOCITY) {
            velocityY = 0;
        }else {
            if (Util::Input::IsKeyPressed(Util::Keycode::UP) and velocityY < MAX_JUMP_VELOCITY) {
                velocityY += additional_jump_force;
            }
        }
    }

    if (is_on_floor()) {
        velocityY = 0;
        this->SetImages(this->AnimationStand);
        isJumping = false;
    }
    mario_y += velocityY * delta_time;**/

    // XX test grow animation XX
    /**if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        if (!is_grow) {
            this->SetLooping(false);
            this->SetImages(this->AnimationGrow);
            is_grow = true;
        }
    }
    if (this->IfAnimationEnds() and is_grow) {
        this->SetLooping(true);
        this->SetImages(this->AnimationStandGrow);
        is_grow = false;
    }**/

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        is_left_key_down = true;
        if (!isRunning) {
            this->SetImages(this->AnimationRun);
            this->SetPlaying(true);
            this->SetLooping(true);
            isRunning = true;
        }
    }else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        is_right_key_down = true;
        if (!isRunning) {
            this->SetImages(this->AnimationRun);
            this->SetPlaying(true);
            this->SetLooping(true);
            isRunning = true;
        }
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        on_jump();
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        on_smalljump();
    }
    /**if (is_on_floor()) {
        this->SetImages(this->AnimationStand);
    }**/
    if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) {
        is_left_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) {
        is_right_key_down = false;
    }
    return on_update(1);
}

void Mario::Increase_Coin(const int coin) {
    this->coin += coin;
}

int Mario::Get_Coin() const {
    return coin;
}

void Mario::Set_Live(const int live) {
    this->live = live;
    if (live == 0) {
        SetImages(AnimationDead);
    }
}

int Mario::Get_Live() const {
    return live;
}

void Mario::Increase_Score(const int score) {
    this->score += score;
}

int Mario::Get_Score() const {
    return score;
}
