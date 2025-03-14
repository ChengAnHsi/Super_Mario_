//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::on_jump() {
    // 只有在垂直速度為 0（代表在地板上或靜止狀態）時才允許跳躍
    if (velocityY == 0) {
        // 當累計的跳躍衝力超過最大跳躍速度，就直接設定最大值
        if (jump_velocity + JUMP_STRENGTH >= MAX_JUMP_VELOCITY) {
            velocityY = MAX_JUMP_VELOCITY;
        } else {
            velocityY += (jump_velocity + JUMP_STRENGTH);
        }
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

    // 根據目前速度更新位置
    mario_y += velocityY * delta;

    // 根據 x 座標決定地板高度
    float ground_level = (mario_x >= -150 && mario_x <= -50) ? 0.0f : -50.0f;

    // 如果 Mario 正在下落，且已經到達或低於地板，就修正位置與速度
    if (velocityY < 0 && mario_y <= ground_level) {
        mario_y = ground_level;
        velocityY = 0;
    } else if (velocityY >= 0) {
        // 當上升時持續施加重力
        calculate_falling_speed();
    }

    this->SetPosition({mario_x, mario_y});
}

float Mario::on_update(int delta) {
    // action: 0(stand) 1(run)
    int direction = is_right_key_down - is_left_key_down;

    // facing update
    if (is_facing_right and direction == -1 || !is_facing_right and direction == 1) {
        // facing left
        m_Transform.scale = glm::vec2{-2, 2};
    }else if (is_facing_right and direction == 1 || is_facing_right and direction == 1) {
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
    move_and_collision(delta);
    return distance;
}
// 檢查是否有區塊在 Mario 下方
bool Mario::has_block_underneath() const {
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    // 假設 x 在 [-150, -50] 時地板高度為 0，其它位置為 -50
    if (mario_x >= -150 && mario_x <= -50) {
        return mario_y <= 0;
    } else {
        return mario_y <= -50;
    }
}
void Mario::calculate_falling_speed() {
    const int BLOCK_SIZE = 30;
    const double SPEED_MAX_RATIO = 1111111.7;
    const double GRAVITY_CONST = 3.0;  // 可根據需求增加數值，例如 1.5 或 2.0

    double max_falling_speed = BLOCK_SIZE * SPEED_MAX_RATIO;

    // 施加重力：上升時 velocityY 會逐步減少，下降後 velocityY 變成負值
    velocityY -= GRAVITY_CONST;

    // 當 Mario 下落時，若速度小於負的最大下降速度，就限制在 -max_falling_speed
    if (velocityY < -max_falling_speed) {
        velocityY = -max_falling_speed;
    }
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
}

int Mario::Get_Live() const {
    return coin;
}

void Mario::Increase_Score(const int score) {
    this->score += score;
}

int Mario::Get_Score() const {
    return coin;
}
