//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"

#include "BlockManager.hpp"
#include "Global.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::OnJump() {
    // 只有在真正落地時才能跳躍
    if (IsOnFloor()) {
        velocityY = JUMP_VELOCITY;
        state = MarioState::Jump;
        // 立刻更新為跳躍圖片
        this->SetImages(AnimationJump);
    }
}

void Mario::OnSmallJump() {
    if (IsOnFloor()) {
        velocityY = SMALL_JUMP_VELOCITY;
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
    }
}

void Mario::OnRun(const float distance) {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;
    mario_x += distance;
    this->SetPosition({mario_x, mario_y});
}

void Mario::MoveAndCollision(const float delta) {
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    // 計算地面高度
    float ground_level = -240.0f;

    if (mario_y <= ground_level) {
        mario_y = ground_level;
        velocityY = 0;
    }

    this->SetPosition({mario_x, mario_y});
}

bool Mario::IsOnFloor() const {
    // 檢查 Mario 是否在對應區間的地面上
    const float mario_x = this->GetPosition().x;
    const float mario_y = this->GetPosition().y;
    constexpr float ground_level = -240.0f;
    return mario_y <= ground_level;
}

void Mario::UpdateAnimation() {
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
    if (!IsOnFloor()) {
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

float Mario::OnUpdate(const float delta) {
    // update moving
    const int direction = is_right_key_down - is_left_key_down;
    const float distance = direction * run_velocity * delta;
    OnRun(distance);

    MoveAndCollision(3 * delta);

    // 每幀更新動畫圖片狀態
    UpdateAnimation();

    return distance;
}
// 檢查是否有區塊在 Mario 下方
bool Mario::HasBlockUnderneath() const {
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    if (mario_x >= -150 && mario_x <= -50) {
        return mario_y <= -240;
    }
    return mario_y <= -270;

}

float Mario::Move() {
    if (is_dead) {
        this->SetImages(this->AnimationDead);
        return 0.0f;
    }

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
        OnJump();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        OnSmallJump();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) {
        is_left_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) {
        is_right_key_down = false;
    }
    return OnUpdate(1);
}

void Mario::IncreaseCoin(const int coin) {
    this->coin += coin;
}

int Mario::GetCoin() const {
    return coin;
}

void Mario::SetLive(const int live) {
    this->live = live;
    if (live == 0) SetImages(AnimationDead);
}

int Mario::GetLive() const {
    return live;
}

void Mario::IncreaseScore(const int score) {
    this->score += score;
}

int Mario::GetScore() const {
    return score;
}

bool PointInRect(const glm::vec2 point, const glm::vec2 rect) {
    return (point.x >= rect.x && point.x <= rect.x + BLOCK_SIZE &&
        point.y >= rect.y && point.y <= rect.y + BLOCK_SIZE);
}

bool Mario::AABBCollides(const glm::vec2 b) const {
    const auto a = this->GetPosition();
    const glm::vec2 mario_size = {this->GetTransform().translation.x, this->GetTransform().translation.y};
    return (a.x < b.x + BLOCK_SIZE && // Collision on Left of a and Right of b
        a.x + mario_size.x > b.x && // Collision on Right of a and Left of b
        a.y < b.y + BLOCK_SIZE && // Collision on Bottom of a and Top of b
        a.y + mario_size.y > b.y); //  Collision on Top of a and Bottom of b
}

/**void Mario::RefixOffset(float width, float height) {
    float mario_x =  this->GetPosition().x;
    float mario_y =  this->GetPosition().y;
    this->SetPosition({mario_x - width, mario_y - height});
}**/