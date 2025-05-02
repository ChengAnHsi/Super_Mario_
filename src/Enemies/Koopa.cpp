#include "Enemies/Koopa.hpp"
#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
bool Koopa::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->is_dead) {
        return false; // No collision if already dead or not visible
    }

    glm::vec2 koopa_pos = GetPosition();
    glm::vec2 koopa_size = m_Drawable->GetSize();
    koopa_size *= KOOPA_MAGNIFICATION;

    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    // Calculate bounding boxes
    float koopa_left = koopa_pos.x - koopa_size.x / 2;
    float koopa_right = koopa_pos.x + koopa_size.x / 2;
    float koopa_top = koopa_pos.y + koopa_size.y / 2;
    float koopa_bottom = koopa_pos.y - koopa_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    // Check for collision
    bool collision_x = (mario_left < koopa_right) && (mario_right > koopa_left);
    bool collision_y = (mario_bottom < koopa_top) && (mario_top > koopa_bottom);

    if (collision_x && collision_y) {
        // Calculate the vertical velocity direction
        bool mario_moving_down = mario->velocityY <= 0;

        // Calculate vertical overlap percentage to determine stomping
        float vertical_overlap = std::min(mario_top, koopa_top) - std::max(mario_bottom, koopa_bottom);
        float mario_height = mario_top - mario_bottom;
        float overlap_percentage = vertical_overlap / mario_height;

        // Check if Mario is above Koopa (stepping on it)
        float overlap_threshold = 12.0f; // Allow a slightly larger overlap

        if ((mario_bottom <= koopa_top + overlap_threshold) &&
            mario_moving_down &&
            overlap_percentage < 0.5f) {

            // Mario is stepping on Koopa from above
            if (!is_shell) {
                // If Koopa is not already a shell, turn it into a shell
                TurnToShell();
                mario->OnKillJump(); // Make Mario bounce
                mario->IncreaseScore(score); // Increase Mario's score
            } else {
                // If Koopa is already a shell, check if it's moving
                if (shell_is_moving) {
                    // Stop the shell if it's moving
                    shell_is_moving = false;
                    mario->OnKillJump(); // Make Mario bounce
                } else {
                    // Kick the shell if it's stationary
                    return KickShell(mario);
                }
            }
            return true;
        } else {
            // Side or bottom collision - Mario gets hurt if not invincible
            if (!is_shell || (is_shell && shell_is_moving)) {
                if (!mario->is_dead && mario->GetLive() > 0) {
                    mario->Die();
                }
            } else if (is_shell && !shell_is_moving) {
                // Kick the shell if it's stationary
                return KickShell(mario);
            }
        }
    }
    return false;
}

void Koopa::TurnToShell() {
    if (!is_shell) {
        is_shell = true;
        shell_is_moving = false;
        shell_timer = 0.0f;

        // Change appearance to shell
        if (GetOverworld() == true) {
            SetImage(AnimationDead, 1000, 0);
        } else {
            SetImage(AnimationUnderWorldDead, 1000, 0);
        }

        // Play a sound effect for shell transformation
        std::shared_ptr<Util::SFX> shell_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.mp3");
        if (shell_sfx) {
            shell_sfx->SetVolume(200);
            shell_sfx->Play();
        }
    }
}

bool Koopa::KickShell(std::shared_ptr<Mario> mario) {
    if (!is_shell) return false;

    shell_is_moving = true;
    shell_timer = 0.0f;

    // Determine shell's movement direction based on Mario's position
    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 koopa_pos = GetPosition();

    // If Mario is on the right of the shell, shell moves left
    // If Mario is on the left of the shell, shell moves right
    isFacingRight = (mario_pos.x < koopa_pos.x);

    // Play a sound effect for shell kick
    std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
    if (kick_sfx) {
        kick_sfx->SetVolume(200);
        kick_sfx->Play();
    }

    return true;
}
void Koopa::Action(const float distance) {
    float Koopa_x = GetPosition().x;
    float Koopa_y = GetPosition().y;
    glm::vec2 Koopa_size = m_Drawable->GetSize();
    Koopa_size *= KOOPA_MAGNIFICATION;

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
        float next_x = Koopa_x + step_distance;  // 計算下一幀位置

        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, Koopa_y}, box);
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
            AABBCollides({next_x, Koopa_y}, block);
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

        Koopa_x = next_x;
        this->SetPosition(Koopa_x, Koopa_y);
        remaining_distance -= step_distance;
    }

    // 如果發生碰撞，反轉方向
    if (collision) {
        isFacingRight = not isFacingRight;
    }
}

bool Koopa::AABBCollides(glm::vec2 Koopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Koopa_pos;
    glm::vec2 Koopa_size = this->m_Drawable->GetSize();
    Koopa_size *= KOOPA_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    X_state = CollisionState::None;
    float aleft = a.x - Koopa_size.x / 2;
    float aright = a.x + Koopa_size.x / 2;
    float atop = a.y + Koopa_size.y / 2;
    float abottom = a.y - Koopa_size.y / 2;

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

bool Koopa::CCDDCollides(glm::vec2 Koopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Koopa_pos;
    glm::vec2 Koopa_size = this->m_Drawable->GetSize();
    Koopa_size *= KOOPA_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    Y_state = CollisionState::None;
    float aleft = a.x - Koopa_size.x / 2;
    float aright = a.x + Koopa_size.x / 2;
    float atop = a.y + Koopa_size.y / 2;
    float abottom = a.y - Koopa_size.y / 2;

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

bool Koopa::GravityAndCollision(const float delta) {
    glm::vec2 Koopa_size = this->m_Drawable->GetSize();
    Koopa_size *= KOOPA_MAGNIFICATION;
    float Koopa_x = this->GetPosition().x;
    float Koopa_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    Koopa_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes){
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({Koopa_x, Koopa_y}, box);

        if (Y_state == CollisionState::Bottom) {
            Koopa_y = box->GetTransform().translation.y + b_size.y / 2 + Koopa_size.y / 2;
            velocityY = 0;
            this->SetPosition(Koopa_x, Koopa_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            Koopa_y = box->GetTransform().translation.y - b_size.y / 2 - Koopa_size.y / 2;
            this->SetPosition(Koopa_x, Koopa_y);
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

        collision = CCDDCollides({Koopa_x, Koopa_y}, block);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            Koopa_y = block->GetTransform().translation.y + b_size.y / 2 + Koopa_size.y / 2;
            velocityY = 0;
            this->SetPosition(Koopa_x, Koopa_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }
    }
    this->SetPosition(Koopa_x, Koopa_y);

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Koopa::UpdateAnimation() {
    //float distance = move_velocity * delta;

    // facing left
    if (isFacingRight == false) {
        m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
        //distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        m_Transform.scale = glm::vec2{-KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    }

	//Action(move_velocity);
}

void Koopa::OnUpdate(const float delta) {
    float distance = GetMoveVelocity() * delta;

    // facing left
    if (isFacingRight == false) {
        //m_Transform.scale = glm::vec2{-Koopa_MAGNIFICATION, Koopa_MAGNIFICATION};
        distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        //m_Transform.scale = glm::vec2{Koopa_MAGNIFICATION, Koopa_MAGNIFICATION};
    }

    GravityAndCollision(3 * delta);

    UpdateAnimation();

    Action(distance);
}

void Koopa::Move(){
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

void Koopa::SetLive(const int live) {
    this->live = live;
    if (live == 0) {
        if (GetOverworld() == true) {
            SetImage(AnimationDead, 1000, 0);
        }else {
            SetImage(AnimationUnderWorldDead, 1000, 0);
        }
    }
}

int Koopa::GetLive() const {
    return live;
}

void Koopa::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Koopa::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Koopa::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Koopa::ClearCollisionBlocks() {
    collision_blocks.clear();
}