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
    if (!isJumping) {
        velocityY = JUMP_VELOCITY;
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
    }
}

void Mario::OnSmallJump() {
    if (!isJumping) {
        velocityY = SMALL_JUMP_VELOCITY;
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
    }
}

void Mario::OnRun(const float distance, std::shared_ptr<BlockManager> m_BM) {
    if (distance == 0) return;

    float mario_x = GetPosition().x;
    float mario_y = GetPosition().y;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    auto background = m_BM->GetBackground();

    const float step = (BLOCK_SIZE + 2) / 4.0f;
    float remaining_distance = distance;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        float next_x = mario_x + step_distance;  // 計算下一幀位置

        bool collision = false;
        for (const auto& block : background) {
            AABBCollides({next_x, mario_y}, block->m_Transform.translation);
            // check next step will collision or not
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }

        // if next step will collision, then do not move
        if (collision) {
            break;
        }

        mario_x = next_x;
        this->SetPosition({ mario_x, mario_y });
        remaining_distance -= step_distance;
    }
}

bool Mario::AABBCollides(glm::vec2 a, glm::vec2 b) {
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    float block_size = BLOCK_SIZE + 2;

    X_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

    float bleft = b.x - block_size / 2;
    float bright = b.x + block_size / 2;
    float btop = b.y + block_size / 2;
    float bbottom = b.y - block_size / 2;

    float EPSILON = 0.0f;  // 允許 x 像素誤差

    bool collisionX = (aleft < bright - EPSILON) && (aright > bleft + EPSILON);
    bool collisionY = (abottom < btop - EPSILON) && (atop > bbottom + EPSILON);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // 計算最小重疊距離
    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return !(X_state == CollisionState::None);
}

bool Mario::CCDDCollides(glm::vec2 a, glm::vec2 b) {
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    float block_size = BLOCK_SIZE + 2;

    Y_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

    float bleft = b.x - block_size / 2;
    float bright = b.x + block_size / 2;
    float btop = b.y + block_size / 2;
    float bbottom = b.y - block_size / 2;

    float EPSILON = 0.0f;  // 允許 x 像素誤差

    bool collisionX = (aleft < bright - EPSILON) && (aright > bleft + EPSILON);
    bool collisionY = (abottom < btop - EPSILON) && (atop > bbottom + EPSILON);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // 計算最小重疊距離
    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return !(Y_state == CollisionState::None);
}

bool Mario::GravityAndCollision(const float delta, std::shared_ptr<BlockManager> m_BM) {
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    float block_size = BLOCK_SIZE + 2;
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;
    auto background = m_BM->GetBackground();

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto& block : background) {
        collision = CCDDCollides({mario_x, mario_y}, block->GetTransform().translation);
        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            mario_y = block->GetTransform().translation.y + block_size / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition({ mario_x, mario_y });
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            mario_y = block->GetTransform().translation.y - block_size / 2 - mario_size.y / 2;
            this->SetPosition({ mario_x, mario_y });
            break;
        }
    }
    if (Y_state == CollisionState::Top) {
        velocityY = 0;
        return true;
    }
    this->SetPosition({ mario_x, mario_y });

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Mario::UpdateAnimation() {
    const int direction = is_right_key_down - is_left_key_down;
    // facing left
    if (direction == -1) {
        m_Transform.scale = glm::vec2{-2, 2};
    }
    // facing right
    if (direction == 1)  {
        m_Transform.scale = glm::vec2{2, 2};
    }

    if (isJumping) {
        // 會影響到跳躍後站立的動畫，註解掉
        // if(state != MarioState::Jump) {
        //     this->SetImages(AnimationJump);
        // }
        state = MarioState::Jump;
    } else {
        // 在地面上根據是否有移動來決定站立或跑步
        if (direction != 0) {
            if(state != MarioState::Run) {
               this->SetImages(AnimationRun);
            }
            isRunning = true;
            state = MarioState::Run;
        } else {
            if(state != MarioState::Stand) {
                this->SetImages(AnimationStand);
            }
            isRunning = false;
            state = MarioState::Stand;
        }
    }
}

float Mario::OnUpdate(const float delta, std::shared_ptr<BlockManager> m_BM) {
    // update moving
    const int direction = is_right_key_down - is_left_key_down;
    const float distance = direction * run_velocity * delta;
    OnRun(distance, m_BM);

    isJumping = GravityAndCollision(3 * delta, m_BM);

    // 每幀更新動畫圖片狀態
    UpdateAnimation();

    return distance;
}

float Mario::Move(std::shared_ptr<BlockManager> m_BM) {
    if (is_dead) {
        //this->SetImages(this->AnimationDead);
        return 0.0f;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        is_left_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        is_right_key_down = true;
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
    return OnUpdate(1, m_BM);
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
