#include "Props/FireFlower.hpp"
#include "Global.hpp"

void FireFlower::AfterCollisionEvents(std::shared_ptr<Mario> mario){
    if(state == PropsState::After_Activated) return;
    if(state == PropsState::Active) {
        SetVisible(false);
        mario->IncreaseScore(1000);
        if (!mario->GetFire()) {
            if (mario->GetGrow()) {
                mario->SetFire(true);
            }else {
                mario->SetGrowingAnimation();
            }
        }
        state = PropsState::After_Activated;
    }
}

void FireFlower::SpawnProps() {
    state = PropsState::Spawning;
    // 對道具大小做偏移 10.0f
    remaining_distance = BLOCK_SIZE - 10.0f;
    velocityY = 3.0f;
}

void FireFlower::Update(float dt) {
    if (state == PropsState::Spawning) {
        float move = velocityY * dt;
        float actual_move = std::min(move, remaining_distance);
        this->SetPosition(this->GetPosition().x, this->GetPosition().y + actual_move);
        remaining_distance -= actual_move;

        if (remaining_distance <= 0.0f) {
            // 浮出完成，進入正常狀態
            state = PropsState::Moving;

            // 初始化正常下落速度，反向
            velocityY = -300.0f;
        }
    } else if (state == PropsState::Moving) {
        Move();
    }
}

bool FireFlower::CCDDCollides(glm::vec2 char_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = char_pos;
    glm::vec2 prop_size = m_Drawable->GetSize();
    prop_size *= PROP_MAGNIFICATION;

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

    bool collisionX = aleft < bright && aright > bleft;
    bool collisionY = abottom < btop && atop > bbottom;

    if (!(collisionX && collisionY)) return false;

    // calculate minimum overlap area
    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
}

bool FireFlower::GravityAndCollision(const float delta) {
    glm::vec2 prop_size = this->m_Drawable->GetSize();
    prop_size *= PROP_MAGNIFICATION;
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
    }
    this->SetPosition(char_x, char_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void FireFlower::OnUpdate(const float delta) {
    GravityAndCollision(3 * delta);
}

void FireFlower::Move(){
    if (state != PropsState::Moving) return;
    OnUpdate(1);
}