#include "../../include/Enemies/Flower.hpp"
#include "Manager/BlockManager.hpp"
#include "../../include/Global.hpp"

Flower::Flower() {
    this -> SetZIndex(-10);
    // Default constructor will calculate the range after SetPosition is called
}

void Flower::UpdateYMovementRange() {
    // Apply your algorithm here
    if (m_Drawable != nullptr) {
        min_y_position = GetPosition().y - (m_Drawable->GetSize().y*3.2);
        max_y_position = GetPosition().y - (m_Drawable->GetSize().y*0.1);
    }
}

void Flower::SetPosition(float x, float y) {
    // First set the position
    Enemy::SetPosition(x, y);
    UpdateYMovementRange();
}

void Flower::Action(const float distance) {
    float Flower_x = GetPosition().x;
    float Flower_y = GetPosition().y;
    glm::vec2 Flower_size = m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    const float step = BLOCK_SIZE / 32.0f;
    float remaining_distance = distance;
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
    Enemy::SetPosition(Flower_x, next_y);
}

bool Flower::AABBCollides(glm::vec2 Flower_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Flower_pos;
    glm::vec2 Flower_size = this->m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    X_state = CollisionState::None;
    float aleft = a.x - Flower_size.x / 2;
    float aright = a.x + Flower_size.x / 2;
    float atop = a.y + Flower_size.y / 2;
    float abottom = a.y - Flower_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    float EPSILON = 0.0f;

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

bool Flower::CCDDCollides(glm::vec2 Flower_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Flower_pos;
    glm::vec2 Flower_size = this->m_Drawable->GetSize();
    Flower_size *= FLOWER_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    Y_state = CollisionState::None;
    float aleft = a.x - Flower_size.x / 2;
    float aright = a.x + Flower_size.x / 2;
    float atop = a.y + Flower_size.y / 2;
    float abottom = a.y - Flower_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    float EPSILON = 0.0f;

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

bool Flower::GravityAndCollision(const float delta) {
    // Disable gravity since we're controlling vertical movement directly
    return false;
}

void Flower::UpdateAnimation() {
    // Update animation based on direction
    if (isFacingUp) {
        // Facing up animation
        m_Transform.scale = glm::vec2{FLOWER_MAGNIFICATION, FLOWER_MAGNIFICATION};
    } else {
        // Facing down animation
        m_Transform.scale = glm::vec2{FLOWER_MAGNIFICATION, FLOWER_MAGNIFICATION};
        // Optional: If you have separate up/down sprites or want to flip
        // m_Transform.scale = glm::vec2{FLOWER_MAGNIFICATION, -FLOWER_MAGNIFICATION};
    }
}

void Flower::OnUpdate(const float delta) {
    float distance = GetMoveVelocity() * delta;
    UpdateAnimation();
    Action(distance);
}

void Flower::Move() {
    if (!GetMoving()) return;
    OnUpdate(1);
    if (is_set_runanimation == false) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
        // Make sure Y range is updated once we have a drawable
        UpdateYMovementRange();
    }
}

void Flower::SetLive(const int live) {
    this->live = live;
    if (live == 0) SetImage(AnimationDead, 100, 0);
}

int Flower::GetLive() const {
    return live;
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