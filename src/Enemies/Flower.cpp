#include "Enemies/Flower.hpp"
#include "Global.hpp"

Flower::Flower() {
    this->SetZIndex(-20);
}
bool Flower::CheckMarioCollision(std::shared_ptr<Mario> mario){
    if (is_dead || !GetVisible() || mario->GetDying()) {
        return false; // No collision if already dead or not visible
    }

    glm::vec2 flower_pos = GetPosition();
    glm::vec2 flower_size = m_Drawable->GetSize();
    flower_size *= FLOWER_MAGNIFICATION;

    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    // Calculate bounding boxes
    float flower_left = flower_pos.x - flower_size.x / 2;
    float flower_right = flower_pos.x + flower_size.x / 2;
    float flower_top = flower_pos.y + flower_size.y / 2;
    float flower_bottom = flower_pos.y - flower_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    // Check for collision
    bool collision_x = (mario_left < flower_right) && (mario_right > flower_left);
    bool collision_y = (mario_bottom < flower_top) && (mario_top > flower_bottom);

    if (collision_x && collision_y) {
        if(mario->GetInvincible()) {
            is_dead = true;
            SetVisible(false);
            return true;
        }
        if (!mario->GetDying() && mario->GetLive() > 0 && !mario->GetTempInvincible()) {
            mario->Die();
            return true;
        }
    }
    return false;
}

void Flower::UpdateYMovementRange() {
    if (m_Drawable != nullptr) {
        min_y_position = GetPosition().y - (m_Drawable->GetSize().y*5.2);
        max_y_position = GetPosition().y - (m_Drawable->GetSize().y*0.1);
    }
}

void Flower::Action(const float distance) {
    float Flower_x = GetPosition().x;
    float Flower_y = GetPosition().y;
    glm::vec2 Flower_size = m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    const float step = BLOCK_SIZE / 48.0f;
    float step_distance = std::min(step, std::abs(distance));

    // Adjust direction based on whether moving up or down
    if (!isFacingUp) {
        step_distance *= -1;
    }

    // Calculate next position
    float next_y = Flower_y + step_distance;

    // Check if we've reached the boundaries
    if (next_y <= min_y_position) {
        next_y = min_y_position;
        isFacingUp = true; // Reverse direction to go up
    } else if (next_y >= max_y_position) {
        next_y = max_y_position;
        isFacingUp = false; // Reverse direction to go down
    }

    // Update position without recalculating the range
    SetPosition(Flower_x, next_y);
}

bool Flower::AABBCollides(glm::vec2 Flower_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Flower_pos;
    glm::vec2 Flower_size = this->m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    X_state = CollisionState::None;
    float aleft = a.x - Flower_size.x / 2;
    float aright = a.x + Flower_size.x / 2;
    float atop = a.y + Flower_size.y / 2;
    float abottom = a.y - Flower_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }

    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return X_state != CollisionState::None;
}

bool Flower::CCDDCollides(glm::vec2 Flower_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Flower_pos;
    glm::vec2 Flower_size = this->m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    Y_state = CollisionState::None;
    float aleft = a.x - Flower_size.x / 2;
    float aright = a.x + Flower_size.x / 2;
    float atop = a.y + Flower_size.y / 2;
    float abottom = a.y - Flower_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }

    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
}

bool Flower::GravityAndCollision(const float delta) {
    // Disable gravity since we're controlling vertical movement directly
    return false;
}

void Flower::UpdateAnimation() {
    m_Transform.scale = glm::vec2{FLOWER_MAGNIFICATION, FLOWER_MAGNIFICATION};
}

void Flower::OnUpdate(const float delta) {
    const float distance = GetMoveVelocity() * delta;
    UpdateAnimation();
    Action(distance);
}

void Flower::Move() {
    if (!GetMoving()) return;
    OnUpdate(1);
    if (is_set_runanimation == false) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
        UpdateYMovementRange();
    }
}

void Flower::SetLive(const int live) {
    this->live = live;
    if (live == 0) {
        is_dead = true;
        SetVisible(false);
    }
}

void Flower::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Flower::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Flower::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Flower::ClearCollisionBlocks() {
    collision_blocks.clear();
}