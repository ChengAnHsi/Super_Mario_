//
// Created by 112590007 on 2025/5/20.
//

#include "Props\Fireball.hpp"
#include "Global.hpp"

void Fireball::Update(float delta) {
    float distance = velocityX * delta;

    // facing left
    if (isFacingRight == false) {
        distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
    }

    isJumping = GravityAndCollision(3 * delta);

    if (!isJumping) {
        Jump();  // 自動連續跳
    }

    if (isFacingRight == false) {
        m_Transform.scale = glm::vec2{-FIREBALL_MAGNIFICATION, FIREBALL_MAGNIFICATION};
    }

    if (isFacingRight)  {
        m_Transform.scale = glm::vec2{FIREBALL_MAGNIFICATION, FIREBALL_MAGNIFICATION};
    }

    Action(distance);
}

bool Fireball::AABBCollides(glm::vec2 char_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = char_pos;
    glm::vec2 prop_size = m_Drawable->GetSize();
    prop_size *= FIREBALL_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    X_state = CollisionState::None;
    float aleft = a.x - prop_size.x / 2;
    float aright = a.x + prop_size.x / 2;
    float atop = a.y + prop_size.y / 2;
    float abottom = a.y - prop_size.y / 2;

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

bool Fireball::CCDDCollides(glm::vec2 char_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = char_pos;
    glm::vec2 prop_size = m_Drawable->GetSize();
    prop_size *= FIREBALL_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    Y_state = CollisionState::None;
    float aleft = a.x - prop_size.x / 2;
    float aright = a.x + prop_size.x / 2;
    float atop = a.y + prop_size.y / 2;
    float abottom = a.y - prop_size.y / 2;

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

void Fireball::Jump() {
    if (!isJumping) {
        velocityY = JUMP_VELOCITY;
        isJumping = true;
    }
}

void Fireball::Action(const float distance) {
    float char_x = GetPosition().x;
    float char_y = GetPosition().y;

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
        float next_x = char_x + step_distance;  // 計算下一幀位置

        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, char_y}, box);
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
            AABBCollides({next_x, char_y}, block);
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

        char_x = next_x;
        this->SetPosition(char_x, char_y);
        remaining_distance -= step_distance;
    }

    // 如果發生碰撞，反轉方向
    if (collision) {
        isFacingRight = not isFacingRight;
    }
}

bool Fireball::GravityAndCollision(const float delta) {
    glm::vec2 prop_size = this->m_Drawable->GetSize();
    prop_size *= FIREBALL_MAGNIFICATION;
    float char_x = this->GetPosition().x;
    float char_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    char_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes){
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({char_x, char_y}, box);

        if (Y_state == CollisionState::Bottom) {
            char_y = box->GetTransform().translation.y + b_size.y / 2 + prop_size.y / 2;
            velocityY = 0;
            this->SetPosition(char_x, char_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            char_y = box->GetTransform().translation.y - b_size.y / 2 - prop_size.y / 2;
            this->SetPosition(char_x, char_y);
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

        collision = CCDDCollides({char_x, char_y}, block);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            char_y = block->GetTransform().translation.y + b_size.y / 2 + prop_size.y / 2;
            velocityY = 0;
            this->SetPosition(char_x, char_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            char_y = block->GetTransform().translation.y - b_size.y / 2 - prop_size.y / 2;
            this->SetPosition(char_x, char_y);
            break;
        }
    }
    if (Y_state == CollisionState::Top) {
        velocityY = 0;
        return true;
    }
    this->SetPosition(char_x, char_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Fireball::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Fireball::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Fireball::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Fireball::ClearCollisionBlocks() {
    collision_blocks.clear();
}

void Fireball::SetFacingRight(bool isFacingRight) {
    this->isFacingRight = isFacingRight;
}