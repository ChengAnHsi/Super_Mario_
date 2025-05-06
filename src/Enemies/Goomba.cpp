#include "Enemies/Goomba.hpp"
#include "Global.hpp"
#include "Mario.hpp"
#include "Util/SFX.hpp"

bool Goomba::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->is_dying) {
        return false; // No collision if already dead or not visible
    }

    glm::vec2 goomba_pos = GetPosition();
    glm::vec2 goomba_size = m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;

    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    // Calculate bounding boxes
    float goomba_left = goomba_pos.x - goomba_size.x / 2;
    float goomba_right = goomba_pos.x + goomba_size.x / 2;
    float goomba_top = goomba_pos.y + goomba_size.y / 2;
    float goomba_bottom = goomba_pos.y - goomba_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    // Check for collision
    bool collision_x = (mario_left < goomba_right) && (mario_right > goomba_left);
    bool collision_y = (mario_bottom < goomba_top) && (mario_top > goomba_bottom);

    if (collision_x && collision_y) {
        // Calculate the vertical velocity direction
        bool mario_moving_down = mario->velocityY <= 0;

        // Calculate vertical overlap percentage to determine stomping
        float vertical_overlap = std::min(mario_top, goomba_top) - std::max(mario_bottom, goomba_bottom);
        float mario_height = mario_top - mario_bottom;
        float overlap_percentage = vertical_overlap / mario_height;

        // Check if Mario is above Goomba (stepping on it)
        // We consider Mario is stepping on Goomba if:
        // 1. Mario's bottom is near Goomba's top
        // 2. Mario is moving downward (falling)
        // 3. The vertical overlap is small compared to Mario's height
        float overlap_threshold = 12.0f; // Allow a slightly larger overlap

        if (mario_bottom <= goomba_top + overlap_threshold && mario_moving_down && overlap_percentage < 0.5f) {
            // Mario is stepping on Goomba from above
            KillGoomba();
            // Make Mario bounce with a small jump
            mario->OnKillJump();
            // Increase Mario's score
            mario->IncreaseScore(score);
            return false;
        }

        // Collision from the side or bottom - Mario gets hurt if not invincible
        if (!mario->is_dying && mario->GetLive() > 0) {
            mario->Die(); // Call our new Die method instead
            return true;
        }
    }
    return false;
}

void Goomba::KillGoomba() {
    if (GetLive() > 0) {
        SetLive(0); // This will trigger the death animation
        is_dead = true; // Set the death flag
        death_timer = 0.0f; // Reset the death timer

        // Play a sound effect for Goomba death
        std::shared_ptr<Util::SFX> death_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/goomba_stomp.wav");
        if (death_sfx) {
            death_sfx->SetVolume(200);
            death_sfx->Play();
        }
    }
}
void Goomba::Action(const float distance) {
    float goomba_x = GetPosition().x;
    float goomba_y = GetPosition().y;
    glm::vec2 goomba_size = m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;

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

bool Goomba::AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = goomba_pos;
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

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
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

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

bool Goomba::GravityAndCollision(const float delta) {
    glm::vec2 goomba_size = this->m_Drawable->GetSize();
    goomba_size *= GOOMBA_MAGNIFICATION;
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
            goomba_y = box->GetTransform().translation.y + b_size.y / 2 + goomba_size.y / 2;
            velocityY = 0;
            this->SetPosition(goomba_x, goomba_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            goomba_y = box->GetTransform().translation.y - b_size.y / 2 - goomba_size.y / 2;
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
            goomba_y = block->GetTransform().translation.y + b_size.y / 2 + goomba_size.y / 2;
            velocityY = 0;
            this->SetPosition(goomba_x, goomba_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
    }
    this->SetPosition(goomba_x, goomba_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Goomba::UpdateAnimation() {
    //float distance = move_velocity * delta;

    // facing left
    if (isFacingRight == false) {
        m_Transform.scale = glm::vec2{-GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
        //distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        m_Transform.scale = glm::vec2{GOOMBA_MAGNIFICATION, GOOMBA_MAGNIFICATION};
    }

	//Action(move_velocity);
}

void Goomba::OnUpdate(const float delta) {
    if (is_dead) {
        death_timer += delta;
        if (death_timer >= DEATH_ANIMATION_TIME) {
            // After the animation time, make the Goomba disappear
            SetVisible(false);
        }
        return; // Skip normal updates if dead
    }

    // Normal update logic for living Goomba
    float distance = GetMoveVelocity() * delta;

    // facing left
    if (isFacingRight == false) {
        distance *= -1;
    }

    GravityAndCollision(3 * delta);
    UpdateAnimation();
    Action(distance);
}
void Goomba::Move(){
    if (!GetMoving()) return;
    OnUpdate(1);
    if (is_set_runanimation == false) {
        if (GetOverworld() == true) {
            SetImage(AnimationRun, 500, 0);
        }else {
            SetImage(AnimationUnderWorldRun, 500, 0);
        }
        is_set_runanimation = true;
    }
}

void Goomba::SetLive(const int live) {
    this->live = live;
    if (live == 0) {
        if (GetOverworld() == true) {
            SetImage(AnimationDead, 1000, 0);
        } else {
            SetImage(AnimationUnderWorldDead, 1000, 0);
        }
        is_dead = true; // Set the death flag
        death_timer = 0.0f; // Reset the death timer
    }
}

int Goomba::GetLive() const {
    return live;
}

void Goomba::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Goomba::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Goomba::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Goomba::ClearCollisionBlocks() {
    collision_blocks.clear();
}