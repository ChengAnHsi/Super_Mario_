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

// void Mario::OnRun(const float distance, std::shared_ptr<BlockManager> m_BM) {
//     if (distance == 0) {
//         return ;
//     }
//     float mario_x =  GetPosition().x;
//     //std::cout << mario_x << std::endl;
//     float mario_y =  GetPosition().y;
//     auto background = m_BM->GetBackground();
//
//     //float mario_sizex = this->m_Drawable->GetSize().x;
//     const float step = BLOCK_SIZE / 4.0f;  // 每次最多移動 1/4 個區塊
//     float remaining_distance = distance;
//
//     while (std::abs(remaining_distance) > 0.0f) {
//         float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
//                                                           : std::max(-step, remaining_distance);
//         this->SetPosition({ mario_x + step_distance, mario_y });
//         for (int i = background.size() - 4; i < background.size(); i++) {
//             if (AABBCollides(background[i]->GetTransform().translation)) {
//                 this->SetPosition({ mario_x, mario_y });  // 遇到碰撞則回退
//                 break;
//             }
//         }
//
//         remaining_distance -= step_distance;
//     }
//     //this->SetPosition({ new_x, mario_y });
//     /**while (std::abs(remaining_distance) > 0.0f) {
//         float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
//                                                           : std::max(-step, remaining_distance);
//         float new_x = mario_x + step_distance;
//         // 檢查 Mario 是否會碰撞到任一方塊
//         for (int i = background.size() - 4; i < background.size(); i++) {
//             if (AABBCollides(background[i]->GetTransform().translation)) {
//                 // 如果發生碰撞，確保 Mario 位置對齊區塊邊界
//
//                 // 向右撞到方塊且角色在方塊左邊
//                 if (distance > 0 && mario_x <= background[i]->GetTransform().translation.x) {
//                     mario_x = background[i]->GetTransform().translation.x - BLOCK_SIZE * 0.8;
//                     this->SetPosition({ mario_x, mario_y });
//                     return;
//                 }
//                 // 向左撞到方塊且角色在方塊右邊
//                 if (distance < 0 && mario_x >= background[i]->GetTransform().translation.x){
//                     mario_x = background[i]->GetTransform().translation.x + BLOCK_SIZE * 0.8;
//                     this->SetPosition({ mario_x, mario_y });
//                     return;
//                 }
//             }
//         }
//
//         // if no collision, mario will move
//         mario_x = new_x;
//         this->SetPosition({ mario_x, mario_y });
//         remaining_distance -= step_distance;
//     }**/
//
//     /**bool is_collision = false;
//     is_collision = AABBCollides(background[i]->GetTransform().translation);
//
//     if (is_collision) {
//     // go left and collision with mario left and block right side
//         if (distance < 0 && mario_x >= background[i]->GetTransform().translation.x) {
//             //mario_x = background[i]->GetTransform().translation.x + BLOCK_SIZE * 0.8;
//             this->SetPosition({ background[i]->GetTransform().translation.x + BLOCK_SIZE * 0.8, mario_y});
//             return ;
//             //break;
//         }
//     // go right and collision with mario right and block left side
//         if (distance > 0 && mario_x <= background[i]->GetTransform().translation.x) {
//             //mario_x = background[i]->GetTransform().translation.x - BLOCK_SIZE * 1.2;
//             this->SetPosition({ background[i]->GetTransform().translation.x - BLOCK_SIZE * 0.8, mario_y});
//             return ;
//             //break;
//         }
//     }**/
//
//
//     //for (const auto & img : background) {}
//     //mario_x += distance;
//     //this->SetPosition({mario_x, mario_y});
// }

void Mario::OnRun(const float distance, std::shared_ptr<BlockManager> m_BM) {
    if (distance == 0) {
        return;
    }
    float mario_x = GetPosition().x;
    float mario_y = GetPosition().y;
    auto background = m_BM->GetBackground();
    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        mario_x += step_distance;
        this->SetPosition({ mario_x, mario_y });

        bool collision = false;
        //for (int i = background.size() - 4; i < background.size(); i++) {
        for (const auto& block : background) {
            collision = AABBCollides(block->GetTransform().translation);
            if (C_state == CollisionState::Left || C_state == CollisionState::Right) {
                //collision = true;
                break;
            }
        }
        if (collision) {
            mario_x -= step_distance;  // 回退
            this->SetPosition({ mario_x, mario_y });
            break;
        }
        remaining_distance -= step_distance;
    }
}

bool Mario::AABBCollides(const glm::vec2 b) {
    const auto a = this->GetPosition();
    glm::vec2 mario_size = this->m_Drawable->GetSize();

    C_state = CollisionState::None;
    bool collisionX = a.x - mario_size.x / 2 < b.x + BLOCK_SIZE / 2 && a.x + mario_size.x / 2 > b.x - BLOCK_SIZE / 2;
    bool collisionY = a.y - mario_size.y / 2 < b.y + BLOCK_SIZE / 2 && a.y + mario_size.y / 2 > b.y - BLOCK_SIZE / 2;

    // no collision
    if (!(collisionX && collisionY)) {
        return false;
    }

    // calculate which collision direction is most
    float ALBR_area = (a.x + mario_size.x / 2) - (b.x - BLOCK_SIZE / 2);
    float ARBL_area = (b.x + BLOCK_SIZE / 2) - (a.x - mario_size.x / 2);
    float ATBB_area = (a.y + mario_size.y / 2) - (b.y - BLOCK_SIZE / 2);
    float ABBT_area = (b.y + BLOCK_SIZE / 2) - (a.y - mario_size.y / 2);
    float min_area = std::max(std::max(std::max(ALBR_area, ARBL_area), ATBB_area), ABBT_area);
    //float max_area = std::min(ALBR_area, ARBL_area);
    if (min_area == ALBR_area) {
        // Collision on Right of a and Left of b
        C_state = CollisionState::Right;
    }
    if (min_area == ARBL_area) {
        // Collision on Left of a and Right of b
        C_state = CollisionState::Left;
    }
    if (min_area == ATBB_area) {
        // Collision on Bottom of a and Top of b
        C_state = CollisionState::Bottom;
    }
    if (min_area == ABBT_area) {
        //  Collision on Top of a and Bottom of b
        C_state = CollisionState::Top;
    }

    return C_state != CollisionState::None;
    /**return (a.x - mario_size.x / 2 < b.x + BLOCK_SIZE / 2 && // Collision on Left of a and Right of b
        a.x + mario_size.x / 2 > b.x - BLOCK_SIZE / 2 && // Collision on Right of a and Left of b
        a.y - mario_size.y / 2 < b.y + BLOCK_SIZE / 2 && // Collision on Bottom of a and Top of b
        a.y + mario_size.y / 2 > b.y - BLOCK_SIZE / 2); //  Collision on Top of a and Bottom of b**/
}

bool Mario::GravityAndCollision(const float delta, std::shared_ptr<BlockManager> m_BM) {
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;
    auto background = m_BM->GetBackground();

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto& block : background) {
        collision = AABBCollides(block->GetTransform().translation);
        if (C_state == CollisionState::Bottom) {
            // collision = true;
            // 防止陷下去
            if ((mario_y - mario_size.y / 2) < (block->GetTransform().translation.y + BLOCK_SIZE / 2)) {
                mario_y = block->GetTransform().translation.y + BLOCK_SIZE / 2 + mario_size.y / 2;
                velocityY = 0;
                this->SetPosition({ mario_x, mario_y });
                return false;  // 碰撞到地面，不在滯空狀態
            }
            // mario_y = block->GetTransform().translation.y - BLOCK_SIZE;
            break;
        }
        if(C_state == CollisionState::Top) {
            //collision = true;
            //mario_y -= velocityY * (delta / 60.0f);  // 回退
            //velocityY = 0;
            //this->SetPosition({ mario_x, mario_y });
            //return false;
            break;
        }
    }
    if (C_state == CollisionState::Top) {
        mario_y -= velocityY * (delta / 60.0f);  // 回退
        velocityY = 0;
    }
    this->SetPosition({ mario_x, mario_y });

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}


// old version
// void Mario::GravityAndCollision(const float delta) {
//     float mario_x = this->GetPosition().x;
//     float mario_y = this->GetPosition().y;
//
//     // 更新垂直速度（根據重力）
//     velocityY += GRAVITY * (delta / 60.0f);
//     mario_y += velocityY * (delta / 60.0f);
//
//     // 計算地面高度
//     float ground_level = -240.0f;
//
//     if (mario_y <= ground_level) {
//         mario_y = ground_level;
//         velocityY = 0;
//     }
//
//     this->SetPosition({mario_x, mario_y});
// }

bool Mario::IsOnFloor() const {
    // 檢查 Mario 是否在對應區間的地面上
    const float mario_x = this->GetPosition().x;
    const float mario_y = this->GetPosition().y;
    constexpr float ground_level = -240.0f;
    return mario_y <= ground_level;
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
