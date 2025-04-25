#include "FlyKoopa.hpp"
#include "BlockManager.hpp"
#include "Global.hpp"

FlyKoopa::FlyKoopa() {
    // Default constructor will calculate the range after SetPosition is called
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

void FlyKoopa::SetPosition(float x, float y) {
    // First set the position
    Enemy::SetPosition(x, y);
    UpdateYMovementRange();
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

    // 如果沒有地面，表示前方有坑洞
    return !has_ground;
}

void FlyKoopa::Action(const float distance) {
    float FlyKoopa_x = GetPosition().x;
    float FlyKoopa_y = GetPosition().y;
    glm::vec2 FlyKoopa_size = m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;

    // 根據不同狀態處理移動
    if (isFlying) {
        // 飛行狀態的垂直移動（像花一樣）
        const float step = BLOCK_SIZE / 32.0f;
        float step_distance = std::min(step, std::abs(distance));

        // 根據面向方向調整方向
        if (!isFacingUp) {
            step_distance *= -1;
        }

        // 計算下一位置
        float next_y = FlyKoopa_y + step_distance;

        // 檢查是否達到邊界
        if (next_y <= min_y_position) {
            next_y = min_y_position;
            isFacingUp = true; // 反向，開始上升
        } else if (next_y >= max_y_position) {
            next_y = max_y_position;
            isFacingUp = false; // 反向，開始下降
        }

        // 更新位置（不重計算範圍）
        Enemy::SetPosition(FlyKoopa_x, next_y);
    }
    else if (isShell && isMovingShell) {
        // 龜殼狀態高速水平移動
        const float shell_step = BLOCK_SIZE / 3.0f; // 龜殼移動速度較快
        float remaining_distance = distance;
        float step_distance = std::min(shell_step, std::abs(distance));

        // 根據面向方向決定移動方向
        if (!isFacingRight) {
            step_distance *= -1;
        }

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
    // 如果是飛行狀態，停用重力（像花一樣）
    if (isFlying) {
        return false;
    }

    // 如果不是飛行狀態，使用重力（像原始的FlyKoopa）
    glm::vec2 FlyKoopa_size = this->m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;
    float FlyKoopa_x = this->GetPosition().x;
    float FlyKoopa_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    FlyKoopa_y += velocityY * (delta / 60.0f);

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
            // 固定在方塊下方開始下墜
            FlyKoopa_y = box->GetTransform().translation.y - b_size.y / 2 - FlyKoopa_size.y / 2;
            this->SetPosition(FlyKoopa_x, FlyKoopa_y);

            // 如果碰撞到FlyKoopa的頂部，轉換為unflykoopa
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
            // 如果碰撞到FlyKoopa的頂部，將其轉換為unfly狀態
            ConvertToUnfly();
        }
    }
    this->SetPosition(FlyKoopa_x, FlyKoopa_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void FlyKoopa::ConvertToUnfly() {
    // 只在飛行狀態下轉換
    if (isFlying) {
        isFlying = false;
        // 更改動畫為非飛行庫巴
        SetImage(AnimationDead, 500, 0);
    }
}
void FlyKoopa::SetMoveVelocity(float speed)
{
    velocityX = speed;

}
void FlyKoopa::ConvertToShell() {
    // 只在非飛行、非龜殼狀態下轉換
    if (!isFlying && !isShell) {
        isShell = true;
        // 設置龜殼動畫
        SetImage(AnimationShell, 500, 0);
        // 停止移動
        velocityX = 0;
        // 確保不會移動
        isMovingShell = false;
    }
}

void FlyKoopa::ActivateShell(bool move_right) {
    if (isShell) {
        // 設定移動方向和速度
        isFacingRight = move_right;
        // 設置移動動畫
        SetImage(AnimationMovingShell, 300, 0);
        // 增加移動速度
        SetMoveVelocity(shell_move_velocity); // 龜殼移動速度比一般庫巴快
        isMovingShell = true;
    }
}

void FlyKoopa::UpdateAnimation() {
    if (isFlying) {
        // 飛行動畫與上/下移動
        if (isFacingUp) {
            // 面向上的動畫
            m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        } else {
            // 面向下的動畫
            m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        }
    }
    else if (isShell) {
        // 龜殼動畫
        if (isMovingShell) {
            // 移動中的龜殼動畫可以旋轉得更快
            m_Transform.scale = isFacingRight ?
                glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION} :
                glm::vec2{-KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        } else {
            // 靜止的龜殼
            m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        }
    }
    else {
        // 標準水平移動動畫
        if (isFacingRight) {
            m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        } else {
            m_Transform.scale = glm::vec2{-KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        }
    }
}

void FlyKoopa::OnUpdate(const float delta) {
    float distance = GetMoveVelocity() * delta;

    if (!isFlying) {
        // 標準水平移動邏輯
        if (isFacingRight == false) {
            distance *= -1;
        }

        // 只有在非飛行狀態下才應用重力
        GravityAndCollision(3 * delta);
    }

    UpdateAnimation();
    Action(distance);
}

void FlyKoopa::Move() {
    if (!GetMoving()) return;
    OnUpdate(1);
    if (is_set_runanimation == false) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
        // 確保在有drawable後更新Y範圍
        UpdateYMovementRange();
    }
}

void FlyKoopa::SetLive(const int live) {
    this->live = live;
    if (live == 0) {
        SetImage(AnimationDead, 100, 0);
        isFlying = false; // 確保死亡時不是飛行狀態
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