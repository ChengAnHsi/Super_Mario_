//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::on_jump() {
    if (velocityY != 0) {
        return ;
    }
    velocityY += jump_velocity;
}

void Mario::on_run(float distance) {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;
    mario_x += distance;
    this->SetPosition({mario_x, mario_y});
}

void Mario::move_and_collision(int delta) {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;
    velocityY += GRAVITY * delta;
    mario_y += velocityY * float(delta);
    if (velocityY > 0) {
        // travel all the blocks
        // collision judge
        // collision judge with line
        //bool is_collide_x = (std::max(mario_x + 30), blocks.right) - std::min(mario_x, blocks.left)
       // <= 30 + (blocks.right - blocks.left(30));
        // video 6:50
    }
    // 暫時的地板
    if (mario_x >= -150 && mario_x <= -50) {
        if (mario_y <= 0) {
            mario_y = 0;
        }
    }else {
        if (mario_y <= -50) {
            mario_y = -50;
        }
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