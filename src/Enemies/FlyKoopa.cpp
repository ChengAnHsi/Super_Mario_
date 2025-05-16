#include "Enemies/FlyKoopa.hpp"

#include <iostream>

#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"

FlyKoopa::FlyKoopa() : Koopa() {
    // Initialize flying Koopa with default values
    isFlying = true;
    isShell = false;
    isMovingShell = false;
    isFacingRight = false;
    isFacingUp = true;
    live = 1;
    velocityY = 0.0f;
    velocityX = 0.0f;

    // Set initial animation
    SetImage(AnimationRun, 500, 0);
    is_set_runanimation = true;

    // Set scale according to direction
    m_Transform.scale = glm::vec2{isFacingRight ? -KOOPA_MAGNIFICATION : KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
}

void FlyKoopa::Action(float distance) {
    // If in flying mode, don't move horizontally
    if (isFlying) {
        return;
    }

    // If in shell mode and not moving, don't move
    if (isShell && !isMovingShell) {
        return;
    }

    // Otherwise, use parent class behavior for horizontal movement
    Koopa::Action(distance);
}

void FlyKoopa::OnUpdate(float delta) {
    delta_time = delta;
    //std::cout << GetPosition().x<<","<<GetPosition().y<<std::endl;

    // Handle death state
    if (GetPosition().y >= -360.0f && dead_state == DeadState::Hit) {
        velocityY += GRAVITY * (delta / 60.0f) * 3.0f;
        float enemy_x = GetPosition().x;
        float enemy_y = GetPosition().y + velocityY * (delta / 60.0f);
        SetPosition(enemy_x, enemy_y);
        if (death_timer >= DEATH_ANIMATION_TIME) {
            SetVisible(false);
        }
        return;
    }

    // Handle gravity and collision
    GravityAndCollision(3 * delta);

    // Update animation based on current state
    UpdateAnimation();

    // If shell and not moving, don't update position
    if (isShell && !isMovingShell) {
        return;
    }

    // Flying behavior: move up and down within range
    if (isFlying) {
        // Update Y movement range based on current position
        UpdateYMovementRange();

        // Move up or down based on current direction
        // Increase the vertical speed for more noticeable movement
        float verticalDistance = 3.0f * delta;  // Increased from 2.0f to 3.0f
        if (!isFacingUp) verticalDistance *= -1;

        float current_y = GetPosition().y;
        float new_y = current_y + verticalDistance;

        // If reaching the top or bottom limit, change direction
        if (new_y >= max_y_position) {
            isFacingUp = false;
            new_y = max_y_position;

            // Debug output
            std::cout << "FlyKoopa reached top limit, moving down. Position: "
                      << GetPosition().x << "," << new_y
                      << " Range: " << min_y_position << "-" << max_y_position << std::endl;
        } else if (new_y <= min_y_position) {
            isFacingUp = true;
            new_y = min_y_position;

            // Debug output
            std::cout << "FlyKoopa reached bottom limit, moving up. Position: "
                      << GetPosition().x << "," << new_y
                      << " Range: " << min_y_position << "-" << max_y_position << std::endl;
        }

        // Update position
        SetPosition(GetPosition().x, new_y);
    } else {
        // Normal walking behavior
        float distance = GetMoveVelocity() * delta;
        if (!isFacingRight) distance *= -1;
        if (isShell) distance *= 3;

        Action(distance);
    }
}
void FlyKoopa::Move() {
    if (!GetMoving()) return;

    OnUpdate(1);

    // Set proper animation if not already set
    if (!is_set_runanimation) {
        SetImage(AnimationRun, 500, 0);
        is_set_runanimation = true;
    }
}

bool FlyKoopa::AABBCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    // Use parent class collision detection
    return Koopa::AABBCollides(flykoopa_pos, box);
}

bool FlyKoopa::CCDDCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    // Use parent class collision detection
    return Koopa::CCDDCollides(flykoopa_pos, box);
}

bool FlyKoopa::GravityAndCollision(float delta) {
    // If flying, don't apply gravity
    if (isFlying) {
        return false;
    }

    // Otherwise, use parent class gravity behavior
    return Koopa::GravityAndCollision(delta);
}

bool FlyKoopa::DetectHole(float next_x, float y) {
    // Check if there's a hole in front of the koopa
    float check_y = y - (m_Drawable->GetSize().y * KOOPA_MAGNIFICATION / 2) - 5.0f;

    bool has_ground = false;

    // Check for ground in collision boxes
    for (const auto &box : collision_boxes) {
        if (!box->GetVisible()) continue;

        glm::vec2 box_pos = box->GetTransform().translation;
        glm::vec2 box_size = box->GetSize();
        box_size.x *= box->GetScale().x;
        box_size.y *= box->GetScale().y;

        float box_left = box_pos.x - box_size.x / 2;
        float box_right = box_pos.x + box_size.x / 2;
        float box_top = box_pos.y + box_size.y / 2;

         if (next_x >= box_left && next_x <= box_right &&
            check_y >= box_top - 5.0f && check_y <= box_top + 5.0f) {
            has_ground = true;
            break;
        }
    }

    // Check for ground in collision blocks
    if (!has_ground) {
        for (const auto &block : collision_blocks) {
            if (!block->GetVisible() || block->GetBroken()) continue;

            glm::vec2 block_pos = block->GetTransform().translation;
            glm::vec2 block_size = block->GetSize();
            block_size.x *= block->GetScale().x;
            block_size.y *= block->GetScale().y;

            float block_left = block_pos.x - block_size.x / 2;
            float block_right = block_pos.x + block_size.x / 2;
            float block_top = block_pos.y + block_size.y / 2;

            if (next_x >= block_left && next_x <= block_right &&
                check_y >= block_top - 5.0f && check_y <= block_top + 5.0f) {
                has_ground = true;
                break;
            }
        }
    }

    return !has_ground;
}

bool FlyKoopa::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->is_dying) return false;

    glm::vec2 koopa_pos = GetPosition();
    glm::vec2 koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize() * MARIO_MAGNIFICATION;

    float koopa_left = koopa_pos.x - koopa_size.x / 2;
    float koopa_right = koopa_pos.x + koopa_size.x / 2;
    float koopa_top = koopa_pos.y + koopa_size.y / 2;
    float koopa_bottom = koopa_pos.y - koopa_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    float EPSILON = 1.0f;
    bool collision_x = (mario_left < koopa_right - EPSILON) && (mario_right > koopa_left + EPSILON);
    bool collision_y = (mario_bottom < koopa_top - EPSILON) && (mario_top > koopa_bottom + EPSILON);

    if (!(collision_x && collision_y)) return false;

    if (mario->GetInvincible()) {
        SetLive(0);
        dead_state = DeadState::Hit;
        SetScale(ENEMY_MAGNIFICATION, -ENEMY_MAGNIFICATION);
        death_timer = 0.0f;
        velocityY = DEATH_JUMP_VELOCITY * 1.5f;

        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        kick_sfx->SetVolume(200);
        kick_sfx->Play();

        mario->IncreaseScore(score);
        return true;
    }

    float overlaps[4] = {
        koopa_right - mario_left,   // Mario from left
        mario_right - koopa_left,   // Mario from right
        koopa_top - mario_bottom,   // Mario from bottom (stomp)
        mario_top - koopa_bottom    // Mario from top
    };

    int min_index = 0;
    float min_overlap = overlaps[0];
    for (int i = 1; i < 4; i++) {
        if (overlaps[i] < min_overlap) {
            min_overlap = overlaps[i];
            min_index = i;
        }
    }

    bool mario_moving_down = mario->velocityY <= 0;
    float horizontal_overlap = std::min(mario_right, koopa_right) - std::max(mario_left, koopa_left);
    float horizontal_percentage = horizontal_overlap / std::min(koopa_size.x, mario_size.x);

    if (min_index == 2 && mario_moving_down && horizontal_percentage > 0.4f) {
        std::shared_ptr<Util::SFX> sound;

        if (isFlying) {
            // Convert from flying to walking
            ConvertToUnfly();
            mario->OnKillJump();
            mario->IncreaseScore(score);
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else if (!isShell) {
            // Convert from walking to shell
            ConvertToShell();
            mario->OnKillJump();
            mario->IncreaseScore(score);
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else if (isMovingShell) {
            // Stop moving shell
            isMovingShell = false;
            SetMoving(false);
            mario->OnKillJump();
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else {
            // Activate shell
            ActivateShell(mario_pos.x < koopa_pos.x);
            mario->OnKillJump();
        }

        if (sound) {
            sound->SetVolume(200);
            sound->Play();
        }
        return false;
    }

    if (!isShell || (isShell && isMovingShell) || ((mario->is_temporarily_invincible == false) && isShell && isMovingShell) ) {
        if (!mario->GetInvincible() && mario->GetLive() > 0) {
            mario->Die();
        }
    } else if (isShell && !isMovingShell) {
        if (min_index == 0 || min_index == 1) {
            // Kick shell
            ActivateShell(mario_pos.x < koopa_pos.x);

            std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
            if (kick_sfx) {
                kick_sfx->SetVolume(200);
                kick_sfx->Play();
            }
            return true;
        }
        if (min_index == 3) {
            return true;
        }
    }
    return true;
}

void FlyKoopa::UpdateAnimation() {

    if (isShell && !isMovingShell) {
        shell_timer += delta_time;

        if (shell_timer >= 500.0f && shell_timer < 800.0f) {
            SetImage({RESOURCE_DIR"/Entities/shell0.png"}, 1000, 0);
        } else if (shell_timer >= 800.0f) {
            isShell = false;
            shell_timer = 0.0f;
            SetMoving(true);

            // Return to walking state, not flying
            SetImage(AnimationDead, 500, 0);
            is_set_runanimation = true;
        }
    }

    // Update scale based on direction
    if (isFlying || isShell) {
        m_Transform.scale = glm::vec2{isFacingRight ? -KOOPA_MAGNIFICATION : KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    }
}

void FlyKoopa::UpdateYMovementRange() {
    float currentX = GetPosition().x;
    float currentY = GetPosition().y;

    // If custom flight height is set, use it
    if (custom_flight_height > 0) {
        float base_y = -100.0f;  // Default ground level if no collision box is found

        // Find the ground level below FlyKoopa
        for (const auto &box : collision_boxes) {
            if (!box->GetVisible()) continue;

            glm::vec2 box_pos = box->GetTransform().translation;
            glm::vec2 box_size = box->GetSize();
            box_size.x *= box->GetScale().x;
            box_size.y *= box->GetScale().y;

            float box_left = box_pos.x - box_size.x / 2;
            float box_right = box_pos.x + box_size.x / 2;
            float box_top = box_pos.y + box_size.y / 2;

            if (currentX >= box_left && currentX <= box_right && box_top > base_y) {
                base_y = box_top;
            }
        }

        // Calculate flight range relative to ground level
        min_y_position = base_y + (m_Drawable->GetSize().y * KOOPA_MAGNIFICATION / 2) + 5.0f;
        max_y_position = min_y_position + custom_flight_height * BLOCK_SIZE;

        // Ensure FlyKoopa is within range initially
        float current_pos_y = GetPosition().y;
        if (current_pos_y < min_y_position) {
            SetPosition(GetPosition().x, min_y_position);
        } else if (current_pos_y > max_y_position) {
            SetPosition(GetPosition().x, max_y_position);
        }
    } else {
        // Default flight range is 3 blocks high
        min_y_position = currentY - default_flight_height * BLOCK_SIZE / 2;
        max_y_position = currentY + default_flight_height * BLOCK_SIZE / 2;
    }
}

void FlyKoopa::SetFlightHeight(float height) {
    custom_flight_height = height;
    UpdateYMovementRange();
}

void FlyKoopa::SetFlightRange(float min_y, float max_y) {
    min_y_position = min_y;
    max_y_position = max_y;
    custom_flight_height = -1.0f; // Disable automatic calculation
}

void FlyKoopa::ConvertToUnfly() {
    if (isFlying) {
        isFlying = false;
        SetMoving(true);
        isFacingRight = false;
        SetImage(AnimationDead, 500, 0);
        is_set_runanimation = true;

        // Play conversion sound
        std::shared_ptr<Util::SFX> sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        if (sound) {
            sound->SetVolume(200);
            sound->Play();
        }
    }
}

void FlyKoopa::ConvertToShell() {
    if (!isShell) {
        isShell = true;
        isMovingShell = false;
        isFlying = false;
        shell_timer = 0.0f;

        SetImage(AnimationShell, 1000, 0);
        SetMoving(false);
        velocityY = 0.0f;

        std::shared_ptr<Util::SFX> shell_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.mp3");
        if (shell_sfx) {
            shell_sfx->SetVolume(200);
            shell_sfx->Play();
        }
    }
}

void FlyKoopa::ActivateShell(bool move_right) {
    if (isShell && !isMovingShell) {
        isMovingShell = true;
        shell_timer = 0.0f;
        SetMoving(true);
        isFacingRight = !move_right;

        SetImage(AnimationMovingShell, 500, 0);

        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        if (kick_sfx) {
            kick_sfx->SetVolume(200);
            kick_sfx->Play();
        }
    }
}

/**void FlyKoopa::SetPosition(float x, float y) {
    Enemy::SetPosition(x, y);
}**/

void FlyKoopa::SetLive(int live) {
    this->live = live;
    if (live == 0) {
        SetImage(AnimationDead, 1000, 0);
        is_dead = true;
        death_timer = 0.0f;
    }
}

int FlyKoopa::GetLive() const {
    return live;
}

bool FlyKoopa::IsFly() const {
    return isFlying;
}

void FlyKoopa::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    collision_boxes.clear();
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void FlyKoopa::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    collision_blocks.clear();
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void FlyKoopa::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void FlyKoopa::ClearCollisionBlocks() {
    collision_blocks.clear();
}