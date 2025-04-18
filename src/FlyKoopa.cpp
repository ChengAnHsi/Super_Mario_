#include "FlyKoopa.hpp"
#include "BlockManager.hpp"
#include "Global.hpp"

void FlyKoopa::Action(const float distance) {
    float FlyKoopa_x = GetPosition().x;
    float FlyKoopa_y = GetPosition().y;
    glm::vec2 FlyKoopa_size = m_Drawable->GetSize();
    FlyKoopa_size *= KOOPA_MAGNIFICATION;

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
        float next_x = FlyKoopa_x + step_distance;  // 計算下一幀位置

        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, FlyKoopa_y}, box);
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
            AABBCollides({next_x, FlyKoopa_y}, block);
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

        FlyKoopa_x = next_x;
        this->SetPosition(FlyKoopa_x, FlyKoopa_y);
        remaining_distance -= step_distance;
    }

    // 如果發生碰撞，反轉方向
    if (collision) {
        isFacingRight = not isFacingRight;
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
    }
    this->SetPosition(FlyKoopa_x, FlyKoopa_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void FlyKoopa::UpdateAnimation() {
    //float distance = move_velocity * delta;

    // facing left
    if (isFacingRight == false) {
        m_Transform.scale = glm::vec2{-KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        //distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    }

	//Action(move_velocity);
}

void FlyKoopa::OnUpdate(const float delta) {
    float distance = GetMoveVelocity() * delta;

    // facing left
    if (isFacingRight == false) {
        //m_Transform.scale = glm::vec2{-FlyKoopa_MAGNIFICATION, FlyKoopa_MAGNIFICATION};
        distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        //m_Transform.scale = glm::vec2{FlyKoopa_MAGNIFICATION, FlyKoopa_MAGNIFICATION};
    }

    GravityAndCollision(3 * delta);

    UpdateAnimation();

    Action(distance);
}

void FlyKoopa::Move(){
    if (!GetMoving()) return;
    OnUpdate(1);
    if (is_set_runanimation == false) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
    }
}

void FlyKoopa::SetLive(const int live) {
    this->live = live;
    if (live == 0) SetImage(AnimationDead, 100, 0);
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