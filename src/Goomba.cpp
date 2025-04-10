#include "Goomba.hpp"

#include "BlockManager.hpp"
#include "Global.hpp"

void Goomba::OnRun(const float distance) {

    float Goomba_x = GetPosition().x;
    float Goomba_y = GetPosition().y;
    glm::vec2 Goomba_size = this->m_Drawable->GetSize();
    Goomba_size *= GOOMBA_MAGNIFICATION;

    // each step move block size / 4
    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        float next_x = Goomba_x + step_distance;  // 計算下一幀位置
		Goomba_x += distance;
    	this->SetPosition(Goomba_x, this->GetPosition().y);
        bool collision = false;
        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, Goomba_y}, box);
            // check next step will collision or not
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }
        Goomba_x = next_x;
        this->SetPosition(Goomba_x, Goomba_y);
        remaining_distance -= step_distance;
    }
}
void Goomba::move() {
    if (!isMoving) return;

    float currentX = this->GetPosition().x;
    currentX += 5.0f;
    this->SetPosition(currentX, this->GetPosition().y);
}
/*
bool Goomba::AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = goomba_pos;
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;

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

bool Goomba::CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = goomba_pos;
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;

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
*/


bool Goomba::GravityAndCollision(const float delta) {
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;
    float goomba_x = this->GetPosition().x;
    float goomba_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    goomba_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes) {
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({goomba_x, goomba_y}, box);

        if (Y_state == CollisionState::Bottom) {
            goomba_y = box->GetTransform().translation.y + b_size.y / 2 + goomba_size.y / 2;
            velocityY = 0;
            this->SetPosition(goomba_x, goomba_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }

            // 固定在方塊下方開始下墜
            goomba_y = box->GetTransform().translation.y - b_size.y / 2 - goomba_size.y / 2;
            this->SetPosition(goomba_x, goomba_y);
            break;
        }

    }

void Goomba::UpdateAnimation(const float delta) {

    float distance = run_velocity * delta;

    // facing left
    if (isfacingright == false) {
        m_Transform.scale = glm::vec2{-GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
        distance *= -1;
    }
    // facing right
    if (isfacingright == true)  {
        m_Transform.scale = glm::vec2{GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
    }

	OnRun(distance);
}

float Goomba::OnUpdate(const float delta) {
    const float distance = 5.0f * delta;
    OnRun(distance);
    return distance;
}