#include "Enemies/FlyKoopa.hpp"
#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"
#include "Enemies/Koopa.hpp"

bool FlyKoopa::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->is_dying) return false;

    glm::vec2 flykoopa_pos = GetPosition();
    glm::vec2 flykoopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize() * MARIO_MAGNIFICATION;

    float flykoopa_left = flykoopa_pos.x - flykoopa_size.x / 2;
    float flykoopa_right = flykoopa_pos.x + flykoopa_size.x / 2;
    float flykoopa_top = flykoopa_pos.y + flykoopa_size.y / 2;
    float flykoopa_bottom = flykoopa_pos.y - flykoopa_size.y / 2;

    float mario_left = mario_pos.x - mario_size.x / 2;
    float mario_right = mario_pos.x + mario_size.x / 2;
    float mario_top = mario_pos.y + mario_size.y / 2;
    float mario_bottom = mario_pos.y - mario_size.y / 2;

    float EPSILON = 1.0f;
    bool collision_x = (mario_left < flykoopa_right - EPSILON) && (mario_right > flykoopa_left + EPSILON);
    bool collision_y = (mario_bottom < flykoopa_top - EPSILON) && (mario_top > flykoopa_bottom + EPSILON);

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
        flykoopa_right - mario_left,   // Mario from left
        mario_right - flykoopa_left,   // Mario from right
        flykoopa_top - mario_bottom,   // Mario from bottom (stomp)
        mario_top - flykoopa_bottom    // Mario from top
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
    float horizontal_overlap = std::min(mario_right, flykoopa_right) - std::max(mario_left, flykoopa_left);
    float horizontal_percentage = horizontal_overlap / std::min(flykoopa_size.x, mario_size.x);

    if (min_index == 2 && mario_moving_down && horizontal_percentage > 0.4f) {
        std::shared_ptr<Util::SFX> sound;
        if (!is_shell) {
            TurnToShell();
            mario->OnKillJump();
            mario->IncreaseScore(score);
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else if (shell_is_moving) {
            shell_is_moving = false;
            SetMoving(false);
            mario->OnKillJump();
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else {
            KickShell(mario);
            mario->OnKillJump();
        }

        if (sound) {
            sound->SetVolume(200);
            sound->Play();
        }
        return false;
    }

    if (!is_shell || (is_shell && shell_is_moving) || (mario->is_temporarily_invincible == false) && is_shell && shell_is_moving) {
        if (!mario->is_invincible && mario->GetLive() > 0) {
            mario->Die();
        }
    } else if (is_shell && !shell_is_moving) {
        if (min_index == 0 || min_index == 1) {
            return KickShell(mario);
        }
        if (min_index == 3) {
            return true;
        }
    }
    return true;
}
FlyKoopa::FlyKoopa() {
    SetImage(AnimationRun, 500, 0);
    SetScale(ENEMY_MAGNIFICATION, ENEMY_MAGNIFICATION);
    SetMoving(false);
}

void FlyKoopa::SetPosition(float x, float y) {
    Enemy::SetPosition(x, y);
    UpdateYMovementRange();
}

void FlyKoopa::UpdateYMovementRange() {
    if (custom_flight_height > 0) {
        // Use custom flight height if set
        float base_y = GetPosition().y;
        float half_height = custom_flight_height * BLOCK_SIZE / 2;
        min_y_position = base_y - half_height;
        max_y_position = base_y + half_height;
    } else {
        // Use default flight height calculation
        float base_y = GetPosition().y;
        float half_height = default_flight_height * BLOCK_SIZE / 2;
        min_y_position = base_y - half_height;
        max_y_position = base_y + half_height;
    }
}

void FlyKoopa::SetFlightHeight(float height) {
    custom_flight_height = height;
    UpdateYMovementRange();
}

void FlyKoopa::SetFlightRange(float min_y, float max_y) {
    min_y_position = min_y;
    max_y_position = max_y;
}

void FlyKoopa::ConvertToUnfly() {
    if (isFlying) {
        isFlying = false;
        velocityY = 0.0f;
        SetImage(AnimationDead, 500, 0);

        // Adjust position to make sure it's on a solid ground
        float current_x = GetPosition().x;
        float current_y = GetPosition().y;

        // Find ground level by simulating gravity
        for (int i = 0; i < 10; i++) { // Try up to 10 blocks down
            float test_y = current_y - i * BLOCK_SIZE;
            SetPosition(current_x, test_y);
            if (!GravityAndCollision(1.0f)) {
                // Found ground
                break;
            }
        }

        std::shared_ptr<Util::SFX> stomp_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        if (stomp_sfx) {
            stomp_sfx->SetVolume(200);
            stomp_sfx->Play();
        }
    }
}

void FlyKoopa::ConvertToShell() {
    if (!isShell) {
        isShell = true;
        isMovingShell = false;
        velocityY = 0.0f;
        SetImage(AnimationShell, 1000, 0);
        SetMoving(false);

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
        isFacingRight = !move_right;
        SetMoving(true);
        SetImage(AnimationMovingShell, 500, 0);

        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        if (kick_sfx) {
            kick_sfx->SetVolume(200);
            kick_sfx->Play();
        }
    }
}

void FlyKoopa::UpdateAnimation() {
    // Handle animation based on current state
    if (isFlying) {
        m_Transform.scale = glm::vec2{isFacingRight ? -ENEMY_MAGNIFICATION : ENEMY_MAGNIFICATION, ENEMY_MAGNIFICATION};
    } else if (isShell) {
        // Shell animation is already set when converting to shell
    } else {
        // Walking animation (unflying state)
        m_Transform.scale = glm::vec2{isFacingRight ? -ENEMY_MAGNIFICATION : ENEMY_MAGNIFICATION, ENEMY_MAGNIFICATION};
    }
}

bool FlyKoopa::DetectHole(float next_x, float y) {
    if (isFlying) return false; // Flying koopas don't care about holes

    // Check if there's ground ahead
    glm::vec2 koopa_size = m_Drawable->GetSize() * ENEMY_MAGNIFICATION;
    float check_x = next_x + (isFacingRight ? koopa_size.x/2 : -koopa_size.x/2);
    float check_y = y - koopa_size.y/2 - 5.0f; // Check slightly below the feet

    // Temporarily move to check position
    glm::vec2 original_pos = GetPosition();
    SetPosition(check_x, check_y);

    // Check for collision with any platform below
    bool found_ground = false;
    for (const auto& box : collision_boxes) {
        if (!box->GetVisible()) continue;

        glm::vec2 b = box->GetTransform().translation;
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        float bleft = b.x - b_size.x/2;
        float bright = b.x + b_size.x/2;
        float btop = b.y + b_size.y/2;

        // Only check if we're above the box
        if (check_x >= bleft && check_x <= bright && check_y > btop) {
            found_ground = true;
            break;
        }
    }

    // Also check blocks
    if (!found_ground) {
        for (const auto& block : collision_blocks) {
            if (!block->GetVisible() || block->GetBroken()) continue;

            glm::vec2 b = block->GetTransform().translation;
            glm::vec2 b_size = block->GetSize();
            b_size.x *= block->GetScale().x;
            b_size.y *= block->GetScale().y;

            float bleft = b.x - b_size.x/2;
            float bright = b.x + b_size.x/2;
            float btop = b.y + b_size.y/2;

            if (check_x >= bleft && check_x <= bright && check_y > btop) {
                found_ground = true;
                break;
            }
        }
    }

    // Restore original position
    SetPosition(original_pos.x, original_pos.y);

    return !found_ground; // Return true if there's a hole (no ground)
}

void FlyKoopa::Action(float distance) {
    if (isShell && !isMovingShell) return;

    float koopa_x = GetPosition().x;
    float koopa_y = GetPosition().y;
    glm::vec2 koopa_size = m_Drawable->GetSize() * ENEMY_MAGNIFICATION;

    // For flying koopas, handle vertical movement first
    if (isFlying) {
        float vertical_distance = GetMoveVelocity() * delta_time * 0.5f;
        if (!isFacingUp) vertical_distance *= -1;

        koopa_y += vertical_distance;

        // Check vertical bounds and reverse direction if needed
        if (koopa_y >= max_y_position) {
            koopa_y = max_y_position;
            isFacingUp = false;
        } else if (koopa_y <= min_y_position) {
            koopa_y = min_y_position;
            isFacingUp = true;
        }

        SetPosition(koopa_x, koopa_y);
    }

    // Handle horizontal movement
    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;

    bool collision = false;
    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ?
            std::min(step, remaining_distance) : std::max(-step, remaining_distance);
        float next_x = koopa_x + step_distance;

        // Check for holes when not flying
        if (!isFlying && DetectHole(next_x, koopa_y)) {
            collision = true;
            break;
        }

        // Check wall collisions
        for (const auto& box : collision_boxes) {
            if (!box->GetVisible()) continue;
            if (AABBCollides({next_x, koopa_y}, box)) {
                if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                    collision = true;
                    break;
                }
            }
        }
        if (collision) break;

        for (const auto& block : collision_blocks) {
            if (!block->GetVisible()) continue;
            if (AABBCollides({next_x, koopa_y}, block)) {
                if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                    collision = true;
                    break;
                }
            }
        }
        if (collision) break;

        // Move to next position
        koopa_x = next_x;
        SetPosition(koopa_x, koopa_y);
        remaining_distance -= step_distance;
    }

    if (collision) isFacingRight = !isFacingRight;
}

bool FlyKoopa::AABBCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = flykoopa_pos;
    glm::vec2 flykoopa_size = m_Drawable->GetSize() * ENEMY_MAGNIFICATION;
    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    X_state = CollisionState::None;
    float aleft = a.x - flykoopa_size.x / 2;
    float aright = a.x + flykoopa_size.x / 2;
    float atop = a.y + flykoopa_size.y / 2;
    float abottom = a.y - flykoopa_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) return false;

    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return X_state != CollisionState::None;
}

bool FlyKoopa::CCDDCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = flykoopa_pos;
    glm::vec2 flykoopa_size = m_Drawable->GetSize() * ENEMY_MAGNIFICATION;
    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    Y_state = CollisionState::None;
    float aleft = a.x - flykoopa_size.x / 2;
    float aright = a.x + flykoopa_size.x / 2;
    float atop = a.y + flykoopa_size.y / 2;
    float abottom = a.y - flykoopa_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) return false;

    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
}

bool FlyKoopa::GravityAndCollision(float delta) {
    // Flying koopas aren't affected by gravity
    if (isFlying) return false;

    glm::vec2 flykoopa_size = m_Drawable->GetSize() * ENEMY_MAGNIFICATION;
    float flykoopa_x = GetPosition().x;
    float flykoopa_y = GetPosition().y;

    velocityY += GRAVITY * (delta / 60.0f);
    flykoopa_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes) {
        if (!box->GetVisible()) continue;

        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({flykoopa_x, flykoopa_y}, box);

        if (Y_state == CollisionState::Bottom) {
            flykoopa_y = box->GetTransform().translation.y + b_size.y / 2 + flykoopa_size.y / 2;
            velocityY = 0;
            SetPosition(flykoopa_x, flykoopa_y);
            return false;
        }
        if (Y_state == CollisionState::Top) {
            flykoopa_y = box->GetTransform().translation.y - b_size.y / 2 - flykoopa_size.y / 2;
            SetPosition(flykoopa_x, flykoopa_y);
            break;
        }
    }

    for (const auto &block : collision_blocks) {
        if (block->GetBroken()) continue;

        glm::vec2 b_size = block->GetSize();
        b_size.x *= block->GetScale().x;
        b_size.y *= block->GetScale().y;

        collision = CCDDCollides({flykoopa_x, flykoopa_y}, block);

        if (Y_state == CollisionState::Bottom) {
            flykoopa_y = block->GetTransform().translation.y + b_size.y / 2 + flykoopa_size.y / 2;
            velocityY = 0;
            SetPosition(flykoopa_x, flykoopa_y);
            return false;
        }
    }

    SetPosition(flykoopa_x, flykoopa_y);
    return !collision;
}

void FlyKoopa::OnUpdate(float delta) {
    delta_time = delta;

    if (GetPosition().y < -360.0f) {
        SetVisible(false);
        return;
    }

    if (!isFlying) {
        GravityAndCollision(3 * delta);
    }

    UpdateAnimation();

    if (isShell && !isMovingShell) return;

    float distance = GetMoveVelocity() * delta;
    if (!isFacingRight) distance *= -1;
    if (isShell && isMovingShell) distance *= 3; // Shell moves faster

    Action(distance);
}

void FlyKoopa::Move() {
    if (!GetMoving()) return;

    OnUpdate(1);

    if (is_set_runanimation == false) {
        if (isFlying) {
            SetImage(AnimationRun, 500, 0);
        } else if (isShell) {
            if (isMovingShell) {
                SetImage(AnimationMovingShell, 500, 0);
            } else {
                SetImage(AnimationShell, 1000, 0);
            }
        } else {
            SetImage(AnimationDead, 500, 0);
        }
        is_set_runanimation = true;
    }
}

void FlyKoopa::SetLive(int live) {
    this->live = live;
    if (live == 0) {
        SetImage(AnimationShell, 1000, 0);
        is_dead = true;
    }
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

void FlyKoopa::SetMoveVelocity(float speed) {
    Enemy::SetMoveVelocity(speed);
}