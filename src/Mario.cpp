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
        // 立刻更新為跳躍圖片
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
    auto background = m_BM->GetBackground();

    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        float next_x = mario_x + step_distance;  // 計算下一幀位置

        bool collision = false;
        for (const auto& block : background) {
            AABBCollides({next_x, mario_y}, block->m_Transform.translation);
            /**if(C_state == CollisionState::Left) {
                this->SetPosition({ block->m_Transform.translation.x / 2 - BLOCK_SIZE / 2 - mario_size.x / 2, mario_y });
            }
            if(C_state == CollisionState::Right) {
                this->SetPosition({ block->m_Transform.translation.x / 2 + BLOCK_SIZE / 2 + mario_size.x / 2, mario_y });
            }**/
            if (C_state == CollisionState::Left || C_state == CollisionState::Right) { // check next step will collision or not
                collision = true;
                //this->SetPosition({ block->m_Transform.translation.x / 2 + BLOCK_SIZE / 2 + mario_size.x / 2, mario_y });
                break;
            }
        }

        if (collision) {
            break; // if next step will collision, then do not move
        }

        mario_x = next_x;
        this->SetPosition({ mario_x, mario_y });
        remaining_distance -= step_distance;
    }
}

bool Mario::AABBCollides(glm::vec2 a, glm::vec2 b) {
//    const auto a = this->GetPosition();
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    float block_size = BLOCK_SIZE;

    C_state = CollisionState::None;
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
    float minOverlap = std::min({bright - aleft, aright - bleft, atop - bbottom, btop - abottom});

    if (minOverlap == bright - aleft) C_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) C_state = CollisionState::Right;
    else if (minOverlap == atop - bbottom) C_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) C_state = CollisionState::Bottom;

    return true;
}

bool Mario::GravityAndCollision(const float delta, std::shared_ptr<BlockManager> m_BM) {
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= 2;
    float block_size = BLOCK_SIZE;
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;
    auto background = m_BM->GetBackground();

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto& block : background) {
        collision = AABBCollides({mario_x, mario_y}, block->GetTransform().translation);
        if (C_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            mario_y = block->GetTransform().translation.y + block_size / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition({ mario_x, mario_y });
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(C_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            mario_y = block->GetTransform().translation.y - block_size / 2 - mario_size.y / 2;
            this->SetPosition({ mario_x, mario_y });
            break;
        }
    }
    if (C_state == CollisionState::Top) {
        mario_y -= velocityY * (delta / 60.0f);  // 回退
        velocityY = 0;
        this->SetPosition({ mario_x, mario_y });
        return true;
    }
    this->SetPosition({ mario_x, mario_y });

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Mario::UpdateAnimation() {
    // 如果不在地面上，狀態必定為 Jump
    const int direction = is_right_key_down - is_left_key_down;
    if (direction == -1) {
        // facing left
        m_Transform.scale = glm::vec2{-2, 2};
    }else if (direction == 1)  {
        // facing right
        m_Transform.scale = glm::vec2{2, 2};
    }

    if (direction != 0) is_facing_right = direction;

    if (isJumping) {
        state = MarioState::Jump;
        this->SetImages(AnimationJump);
    } else {
        // 在地面上根據是否有移動來決定站立或跑步
        if (direction != 0) {
            if(isRunning == false) {
                this->SetImages(AnimationRun);
            }
            isRunning = true;
            state = MarioState::Run;
        } else {
            if(isRunning || isJumping) {
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
        this->SetImages(this->AnimationDead);
        return 0.0f;
    }

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
