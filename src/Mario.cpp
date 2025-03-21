//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::on_jump() {
    // 只有在真正落地時才能跳躍
    if (is_on_floor()) {
        velocityY = JUMP_VELOCITY;
        state = MarioState::Jump;
        // 立刻更新為跳躍圖片
        this->SetImages(AnimationJump);
    }
}


void Mario::on_smalljump() {
    if (is_on_floor()) {
        velocityY = SMALL_JUMP_VELOCITY;
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
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

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    // 計算地面高度
    float ground_level = (mario_x >= -150 && mario_x <= -50) ? -240.0f : -270.0f;

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
void Mario::update_animation() {
    // 如果不在地面上，狀態必定為 Jump
    int direction = is_right_key_down - is_left_key_down;
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
    if (!is_on_floor()) {
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
    } else {
        // 在地面上根據是否有移動來決定站立或跑步
        int direction = is_right_key_down - is_left_key_down;
        if (direction != 0) {
            state = MarioState::Run;
            this->SetImages(AnimationRun);
        } else {
            state = MarioState::Stand;
            this->SetImages(AnimationStand);
        }
    }
}

float Mario::on_update(int delta) {
    // 更新移動
    int direction = is_right_key_down - is_left_key_down;
    float distance = direction * run_velocity * delta;
    on_run(distance);  // 移動的程式碼

    move_and_collision(3*delta);

    // 每幀更新動畫圖片狀態
    update_animation();

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
        this->SetImages(this- q>AnimationStand);
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
