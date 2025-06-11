#include "Enemies/FlyKoopa.hpp"
#include "Enemies/Enemy.hpp"
#include "Global.hpp"
#include "Util/SFX.hpp"

bool FlyKoopa::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->GetDying()) return false;

    glm::vec2 koopa_pos = GetPosition();
    glm::vec2 koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize() * MARIO_MAGNIFICATION;

    float koopa_left = koopa_pos.x - koopa_size.x / 2;
    float koopa_right = koopa_pos.x + koopa_size.x / 2;
    float koopa_top = koopa_pos.y + koopa_size.y / 2;
    float koopa_bottom = koopa_pos.y - koopa_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    float EPSILON = 1.0f;
    bool collision_x = (mario_left < koopa_right - EPSILON) && (mario_right > koopa_left + EPSILON);
    bool collision_y = (mario_bottom < koopa_top - EPSILON) && (mario_top > koopa_bottom + EPSILON);

    if (!(collision_x && collision_y)) return false;

    if (mario->GetInvincible()) {
        SetLive(0);
        dead_state = DeadState::Hit;
        SetScale(ENEMY_MAGNIFICATION, -ENEMY_MAGNIFICATION);
        death_timer = 0.0f;
        velocityY = DEATH_JUMP_VELOCITY * 1.5f;
        SetImage(AnimationShell,1000,0);
        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        kick_sfx->SetVolume(200);
        kick_sfx->Play();

        mario->IncreaseScore(score);
        return true;
    }

    float overlaps[4] = {
        koopa_right - mario_left,   // Mario from left
        mario_right - koopa_left,   // Mario from right
        koopa_top - mario_bottom,   // Mario from bottom (stomp)
        mario_top - koopa_bottom    // Mario from top
    };

    int min_index = 0;
    float min_overlap = overlaps[0];
    for (int i = 1; i < 4; i++) {
        if (overlaps[i] < min_overlap) {
            min_overlap = overlaps[i];
            min_index = i;
        }
    }

    bool mario_moving_down = mario->GetVelocityY() <= 0;
    float horizontal_overlap = std::min(mario_right, koopa_right) - std::max(mario_left, koopa_left);
    float horizontal_percentage = horizontal_overlap / std::min(koopa_size.x, mario_size.x);

    if (min_index == 2 && mario_moving_down && horizontal_percentage > -0.5f) {
        mario->OnKillJump();
        SetVisible(false);
        is_dead = true;
        inside_self->SetVelocityY(0.0f);
        inside_self->SetLive(1);
        inside_self->SetFromFly();
        inside_self->SetVisible(true);
        inside_self->SetImage(AnimationDead,1000,0);
        inside_self->SetPosition(GetPosition().x, GetPosition().y);
        return false;
    }
    if (mario->GetTempInvincible() == false && mario->GetLive() > 0) mario->Die();
    return true;
}

void FlyKoopa::UpdateYMovementRange() {
    if (m_Drawable != nullptr) {
        float sprite_height = m_Drawable->GetSize().y;

        if (custom_flight_height > 0) {
            // Use custom flight height setting if available
            float flight_distance = custom_flight_height * BLOCK_SIZE;
            min_y_position = GetPosition().y - flight_distance;
            max_y_position = GetPosition().y - (sprite_height * 0.1);
        } else {
            // Default calculation similar to Flower
            min_y_position = GetPosition().y - (sprite_height * default_flight_height);
            max_y_position = GetPosition().y - (sprite_height * 0.1);
        }
    }
}

void FlyKoopa::SetInsideKoopa(std::shared_ptr<Koopa> inside_koopa) {
    inside_self = inside_koopa;
}

void FlyKoopa::SetFlightHeight(float height) {
    // Set custom flight height in blocks
    custom_flight_height = height;
    UpdateYMovementRange();
}

void FlyKoopa::SetFlightRange(float min_y, float max_y) {
    // Set explicit min and max y positions
    min_y_position = min_y;
    max_y_position = max_y;
}

bool FlyKoopa::DetectHole(float next_x, float y) {
    // 檢查下一步是否會掉入坑洞（即下方沒有任何碰撞體）
    float check_y = y - BLOCK_SIZE; // 檢查角色腳下一格
    bool has_ground = false;

    // 設置檢測範圍（寬度約為角色的一半）
    float check_width = m_Drawable->GetSize().x * KOOPA_MAGNIFICATION * 0.4f;

    // 檢查每個碰撞方塊
    for (const auto& box : collision_boxes) {
        if (!box->GetVisible()) continue;

        glm::vec2 box_pos = box->GetTransform().translation;
        glm::vec2 box_size = box->GetSize();
        box_size.x *= box->GetScale().x;
        box_size.y *= box->GetScale().y;

        // 檢查方塊是否在角色下方且有重疊
        if (box_pos.y < y &&
            box_pos.y + box_size.y/2 >= check_y &&
            box_pos.x - box_size.x/2 <= next_x + check_width &&
            box_pos.x + box_size.x/2 >= next_x - check_width) {
            has_ground = true;
            break;
        }
    }

    // 如果沒有找到地面，也檢查碰撞方塊
    if (!has_ground) {
        for (const auto& block : collision_blocks) {
            if (block->GetBroken()) continue;

            glm::vec2 block_pos = block->GetTransform().translation;
            glm::vec2 block_size = block->GetSize();
            block_size.x *= block->GetScale().x;
            block_size.y *= block->GetScale().y;

            if (block_pos.y < y &&
                block_pos.y + block_size.y/2 >= check_y &&
                block_pos.x - block_size.x/2 <= next_x + check_width &&
                block_pos.x + block_size.x/2 >= next_x - check_width) {
                has_ground = true;
                break;
            }
        }
    }
    return !has_ground;
    return false;
}

void FlyKoopa::Action(const float distance) {
    float FlyKoopa_x = GetPosition().x;
    float FlyKoopa_y = GetPosition().y;
    glm::vec2 FlyKoopa_size = m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;
    if (isFlying) {
        const float step = BLOCK_SIZE / 32.0f;
        float step_distance = std::min(step, std::abs(distance));
        if (!isFacingUp) step_distance *= -1;
        float next_y = FlyKoopa_y + step_distance;

        // 檢查是否達到邊界
        if (next_y <= min_y_position) {
            next_y = min_y_position;
            isFacingUp = true;
        } else if (next_y >= max_y_position) {
            next_y = max_y_position;
            isFacingUp = false;
        }

        // 更新位置（不重計算範圍）
        SetPosition(FlyKoopa_x, next_y);
    }

    else if (isShell && isMovingShell) {
        const float shell_step = BLOCK_SIZE / 3.0f;
        float remaining_distance = distance;
        float step_distance = std::min(shell_step, std::abs(distance));
        if (!isFacingRight) step_distance *= -1;

        bool collision = false;
        while (std::abs(remaining_distance) > 0.0f) {
            float step_distance = (remaining_distance > 0.0f) ? std::min(shell_step, remaining_distance)
                                                            : std::max(-shell_step, remaining_distance);

            // 根據面向方向調整
            if (!isFacingRight) {
                step_distance *= -1;
            }

            float next_x = FlyKoopa_x + step_distance;

            // 檢查與方塊的碰撞
            for (const auto& box : collision_boxes) {
                if (box->GetVisible() == false) continue;

                AABBCollides({next_x, FlyKoopa_y}, box);
                if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                    collision = true;
                    break;
                }
            }
            if (collision) break;

            FlyKoopa_x = next_x;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);
            remaining_distance -= step_distance;
        }

        // 發生碰撞時反轉方向
        if (collision) {
            isFacingRight = !isFacingRight;
        }
    }
    else {
        // 普通狀態的水平移動
        const float step = BLOCK_SIZE / 4.0f;
        float remaining_distance = distance;
        float step_distance = std::min(step, std::abs(distance));

        // 根據面向方向決定移動方向
        if (!isFacingRight) {
            step_distance *= -1;
        }

        bool collision = false;
        while (std::abs(remaining_distance) > 0.0f) {
            float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                            : std::max(-step, remaining_distance);

            // 根據面向方向調整
            if (!isFacingRight) {
                step_distance *= -1;
            }

            float next_x = FlyKoopa_x + step_distance;

            // 檢查前方是否有坑洞（只在非飛行模式且非龜殼模式時）
            if (!isFlying && !isShell && DetectHole(next_x, FlyKoopa_y)) {
                isFacingRight = !isFacingRight; // 改變方向
                step_distance *= -1; // 反向移動
                next_x = FlyKoopa_x + step_distance; // 重新計算下一個位置
            }

            // 檢查與方塊的碰撞
            for (const auto& box : collision_boxes) {
                if (box->GetVisible() == false) continue;

                AABBCollides({next_x, FlyKoopa_y}, box);
                if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                    collision = true;
                    break;
                }
            }

            if (collision) break;

            // 檢查與碰撞方塊的碰撞
            for (const auto& block : collision_blocks) {
                if (block->GetVisible() == false) continue;

                AABBCollides({next_x, FlyKoopa_y}, block);
                if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                    collision = true;
                    break;
                }
            }

            if (collision) break;

            FlyKoopa_x = next_x;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);
            remaining_distance -= step_distance;
        }

        // 發生碰撞時反轉方向
        if (collision) {
            isFacingRight = !isFacingRight;
        }
    }
}

bool FlyKoopa::AABBCollides(glm::vec2 FlyKoopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = FlyKoopa_pos;
    glm::vec2 FlyKoopa_size = this->m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    X_state = CollisionState::None;
    float aleft = a.x - FlyKoopa_size.x / 2;
    float aright = a.x + FlyKoopa_size.x / 2;
    float atop = a.y + FlyKoopa_size.y / 2;
    float abottom = a.y - FlyKoopa_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    float EPSILON = 0.0f;  // 允許 0.0 pixel 誤差

    bool collisionX = (aleft < bright - EPSILON) && (aright > bleft + EPSILON);
    bool collisionY = (abottom < btop - EPSILON) && (atop > bbottom + EPSILON);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // calculate minimum overlap area
    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return X_state != CollisionState::None;
}

bool FlyKoopa::CCDDCollides(glm::vec2 FlyKoopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = FlyKoopa_pos;
    glm::vec2 FlyKoopa_size = this->m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    Y_state = CollisionState::None;
    float aleft = a.x - FlyKoopa_size.x / 2;
    float aright = a.x + FlyKoopa_size.x / 2;
    float atop = a.y + FlyKoopa_size.y / 2;
    float abottom = a.y - FlyKoopa_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    float EPSILON = 0.0f;  // 允許 0.0 pixel 誤差

    bool collisionX = (aleft < bright - EPSILON) && (aright > bleft + EPSILON);
    bool collisionY = (abottom < btop - EPSILON) && (atop > bbottom + EPSILON);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // calculate minimum overlap area
    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
}

bool FlyKoopa::GravityAndCollision(const float delta) {
    if (isFlying) {
        return false;
    }

    glm::vec2 FlyKoopa_size = this->m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;

    float FlyKoopa_x = this->GetPosition().x;
    float FlyKoopa_y = this->GetPosition().y;

    velocityY += GRAVITY * (delta / 60.0f);
    FlyKoopa_y += velocityY * (delta / 60.0f);

    if (is_dead) {
        this->SetPosition(FlyKoopa_x, FlyKoopa_y);
        if (FlyKoopa_y < -50) {
            SetVisible(false);
        }
        return true;
    }

    bool collision = false;
    for (const auto &box : collision_boxes){
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({FlyKoopa_x, FlyKoopa_y}, box);

        if (Y_state == CollisionState::Bottom) {
            FlyKoopa_y = box->GetTransform().translation.y + b_size.y / 2 + FlyKoopa_size.y / 2;
            velocityY = 0;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            FlyKoopa_y = box->GetTransform().translation.y - b_size.y / 2 - FlyKoopa_size.y / 2;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);

            ConvertToUnfly();
            break;
        }
    }
    for (const auto &block : collision_blocks) {
        // block had already destroyed
        if(block->GetBroken() == true) {
            continue;
        }
        glm::vec2 b_size = block->GetSize();
        b_size.x *= block->GetScale().x;
        b_size.y *= block->GetScale().y;

        collision = CCDDCollides({FlyKoopa_x, FlyKoopa_y}, block);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            FlyKoopa_y = block->GetTransform().translation.y + b_size.y / 2 + FlyKoopa_size.y / 2;
            velocityY = 0;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }

        if (Y_state == CollisionState::Top) {
            ConvertToUnfly();
        }
    }
    this->SetPosition(FlyKoopa_x, FlyKoopa_y);

    return !collision;
}

void FlyKoopa::ConvertToUnfly() {
    // 只在飛行狀態下轉換
    if (isFlying) {
        isFlying = false;
        SetImage(AnimationDead, 500, 0);
    }
}

void FlyKoopa::UpdateAnimation() {
    m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
}

void FlyKoopa::OnUpdate(const float delta) {
    if (is_dead) {
        death_timer += delta;
        if(GetPosition().y >= -360.0f && (dead_state == DeadState::Hit || dead_state == DeadState::Shot)){
            velocityY += GRAVITY * (delta / 60.0f) * 3.0f;
            float enemy_x = GetPosition().x;
            float enemy_y = GetPosition().y + velocityY * (delta / 60.0f);
            SetPosition(enemy_x, enemy_y);
        }
        if (death_timer >= DEATH_ANIMATION_TIME) {
            SetVisible(false);
        }
        return;
    }

    float distance = GetMoveVelocity() * delta;

    if (!isFlying) {
        if (isFacingRight == false) {
            distance *= -1;
        }

        GravityAndCollision(delta);
    }

    UpdateAnimation();
    Action(distance);
}

void FlyKoopa::Move() {
    OnUpdate(1);
    if (is_set_runanimation == false) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
        // 確保在有drawable後更新Y範圍
        UpdateYMovementRange();
    }
}

void FlyKoopa::SetLive(const int live) {
    // If it is not dead yet, turn it into a dead shell.
    if((dead_state == DeadState::Shot || dead_state == DeadState::Hit) && this->live != 0) {
        this->is_dead = true;
        SetVisible(false);

        inside_self->SetLive(0);
        inside_self->SetFromFly();
        inside_self->SetVisible(true);
        inside_self->SetImage(AnimationShell,1000,0);
        inside_self->SetPosition(GetPosition().x, GetPosition().y);
        inside_self->SetVelocityY(0.0f);
        inside_self->SetDeadState(dead_state);
        inside_self->SetScale(KOOPA_MAGNIFICATION,-KOOPA_MAGNIFICATION);
    }

    this->live = live;

    if (live == 0) {
        this->is_dead = true;
        if(GetPosition().y >= -360.0f && (dead_state == DeadState::Shot || dead_state == DeadState::Hit)) {
            float delta = 1.0f;
            velocityY += GRAVITY * (delta / 60.0f) * 3.0f;

            float enemy_x = GetPosition().x;
            float enemy_y = GetPosition().y + velocityY * (delta / 60.0f);
            SetPosition(enemy_x, enemy_y);
        }else {
            SetImage(AnimationDead, 100, 0);
            isFlying = false; // 確保死亡時不是飛行狀態
        }
    }
}

int FlyKoopa::GetLive() const {
    return live;
}

void FlyKoopa::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void FlyKoopa::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void FlyKoopa::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void FlyKoopa::ClearCollisionBlocks() {
    collision_blocks.clear();
}