#include "MagicMushroom.hpp"
#include "Global.hpp"

void MagicMushroom::AfterCollisionEvents(){

}

void MagicMushroom::SpawnProps() {
    state = PropsState::Spawning;
    // 對道具大小作偏移 10.0f
    remaining_distance = BLOCK_SIZE - 10.0f;
    velocityY = 3.0f; // 向上速度
}

void MagicMushroom::Update(float dt) {
    if (state == PropsState::Spawning) {
        float move = velocityY * dt;
        float actual_move = std::min(move, remaining_distance);
        this->SetPosition(this->GetPosition().x, this->GetPosition().y + actual_move);
        remaining_distance -= actual_move;

        if (remaining_distance <= 0.0f) {
            // 浮出完成，進入正常狀態
            state = PropsState::Active;

            // 初始化正常下落速度，反向
            velocityY = -300.0f;
        }
    } else if (state == PropsState::Active) {
        Move();
    }
}

void MagicMushroom::Action(const float distance) {
    float goomba_x = GetPosition().x;
    float goomba_y = GetPosition().y;
    glm::vec2 goomba_size = m_Drawable->GetSize();
    goomba_size *= PROP_MAGNIFICATION;

    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;
    float step_distance = std::min(step, std::abs(distance));

    // go left or right
    if (!isFacingRight){
        step_distance *= -1;
    }

    bool collision = false;
    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        float next_x = goomba_x + step_distance;  // 計算下一幀位置

        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, goomba_y}, box);
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
        for (const auto& block : collision_blocks) {
            // box had already destroyed
            if (block->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, goomba_y}, block);
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

        goomba_x = next_x;
        this->SetPosition(goomba_x, goomba_y);
        remaining_distance -= step_distance;
    }

    // 如果發生碰撞，反轉方向
    if (collision) {
        isFacingRight = not isFacingRight;
    }
}

bool MagicMushroom::AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = goomba_pos;
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= PROP_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    X_state = CollisionState::None;
    float aleft = a.x - goomba_size.x / 2;
    float aright = a.x + goomba_size.x / 2;
    float atop = a.y + goomba_size.y / 2;
    float abottom = a.y - goomba_size.y / 2;

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

bool MagicMushroom::CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = goomba_pos;
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= PROP_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    Y_state = CollisionState::None;
    float aleft = a.x - goomba_size.x / 2;
    float aright = a.x + goomba_size.x / 2;
    float atop = a.y + goomba_size.y / 2;
    float abottom = a.y - goomba_size.y / 2;

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

bool MagicMushroom::GravityAndCollision(const float delta) {
    glm::vec2 prop_size = this->m_Drawable->GetSize();
    prop_size *= PROP_MAGNIFICATION;
    float goomba_x = this->GetPosition().x;
    float goomba_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    goomba_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes){
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({goomba_x, goomba_y}, box);

        if (Y_state == CollisionState::Bottom) {
            goomba_y = box->GetTransform().translation.y + b_size.y / 2 + prop_size.y / 2;
            velocityY = 0;
            this->SetPosition(goomba_x, goomba_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            goomba_y = box->GetTransform().translation.y - b_size.y / 2 - prop_size.y / 2;
            this->SetPosition(goomba_x, goomba_y);
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

        collision = CCDDCollides({goomba_x, goomba_y}, block);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            goomba_y = block->GetTransform().translation.y + b_size.y / 2 + prop_size.y / 2;
            velocityY = 0;
            this->SetPosition(goomba_x, goomba_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
    }
    this->SetPosition(goomba_x, goomba_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void MagicMushroom::UpdateAnimation() {
    //float distance = move_velocity * delta;

    // facing left
    // if (isFacingRight == false) {
    //     m_Transform.scale = glm::vec2{-GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
    //     //distance *= -1;
    // }
    // facing right
    // if (isFacingRight == true)  {
    //     m_Transform.scale = glm::vec2{GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
    // }

	//Action(move_velocity);
}

void MagicMushroom::OnUpdate(const float delta) {
    float distance = velocityX * delta;

    // facing left
    if (isFacingRight == false) {
        //m_Transform.scale = glm::vec2{-GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
        distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        //m_Transform.scale = glm::vec2{GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
    }

    GravityAndCollision(3 * delta);

    UpdateAnimation();

    Action(distance);
}

void MagicMushroom::Move(){
    if (state != PropsState::Active) return;
    OnUpdate(1);
}