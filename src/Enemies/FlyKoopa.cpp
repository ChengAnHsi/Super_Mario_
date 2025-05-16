#include "Enemies/FlyKoopa.hpp"
#include <iostream>
#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"

FlyKoopa::FlyKoopa() : Koopa() {
    isFlying = true;
    isShell = false;
    isMovingShell = false;
    isFacingRight = false;
    isFacingUp = true;
    live = 1;
    velocityY = 0.0f;
    velocityX = 0.0f;
    m_Transform.scale = glm::vec2{isFacingRight ? -KOOPA_MAGNIFICATION : KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    UpdateYMovementRange();
    SetImage(AnimationFly, 500, 2); // Set initial flying animation
    current_animation = AnimationState::Flying;
}

void FlyKoopa::UpdateYMovementRange() {
    if (m_Drawable != nullptr) {
        float basePos = GetPosition().y;
        if (custom_flight_height > 0) {
            float range = custom_flight_height * BLOCK_SIZE;
            min_y_position = basePos - range / 2;
            max_y_position = basePos + range / 2;
        } else {
            float range = default_flight_height * BLOCK_SIZE;
            min_y_position = basePos - range / 2;
            max_y_position = basePos + range / 2;
        }
    }
}

void FlyKoopa::Action(float distance) {
    if (isFlying) {
        SetImage(AnimationFly, 500, 2); // Set initial flying animation
        float koopa_x = GetPosition().x;
        float koopa_y = GetPosition().y;
        const float step = BLOCK_SIZE / 48.0f;
        float step_distance = std::min(step, std::abs(distance));
        if (!isFacingUp) step_distance *= -1;
        float next_y = koopa_y + step_distance;
        if (next_y <= min_y_position) {
            next_y = min_y_position;
            isFacingUp = true;
        } else if (next_y >= max_y_position) {
            next_y = max_y_position;
            isFacingUp = false;
        }
        Enemy::SetPosition(koopa_x, next_y);
        return;
    }
    if (isShell && !isMovingShell) return;
    Koopa::Action(distance);
}

void FlyKoopa::OnUpdate(float delta) {
    delta_time = delta;
    if (GetPosition().y >= -360.0f && dead_state == DeadState::Hit) {
        velocityY += GRAVITY * (delta / 60.0f) * 3.0f;
        float enemy_x = GetPosition().x;
        float enemy_y = GetPosition().y + velocityY * (delta / 60.0f);
        SetPosition(enemy_x, enemy_y);
        if (death_timer >= DEATH_ANIMATION_TIME) SetVisible(false);
        return;
    }
    if (!isFlying) GravityAndCollision(3 * delta);
    UpdateAnimation();
    if (isShell && !isMovingShell) return;
    if (isFlying) {
        float distance = GetMoveVelocity() * delta;
        Action(distance);
    } else {
        float distance = GetMoveVelocity() * delta;
        if (!isFacingRight) distance *= -1;
        if (isShell) distance *= 3;
        Action(distance);
    }
}

void FlyKoopa::Move() {
    if (!GetMoving()) return;
    OnUpdate(1);
}

bool FlyKoopa::GravityAndCollision(float delta) {
    if (isFlying) return false;
    return Koopa::GravityAndCollision(delta);
}

void FlyKoopa::SetFlightHeight(float height) {
    custom_flight_height = height;
    if (isFlying) UpdateYMovementRange();
}

void FlyKoopa::SetFlightRange(float min_y, float max_y) {
    min_y_position = min_y;
    max_y_position = max_y;
    custom_flight_height = -1.0f;
}

void FlyKoopa::UpdateAnimation() {
    if (isFlying) {
        if (current_animation != AnimationState::Flying) {
            SetImage(AnimationFly, 500, 0);
            current_animation = AnimationState::Flying;
        }
    } else if (isShell) {
        if (isMovingShell) {
            if (current_animation != AnimationState::MovingShell) {
                SetImage(AnimationMovingShell, 500, 0);
                current_animation = AnimationState::MovingShell;
            }
        } else {
            shell_timer += delta_time;
            if (shell_timer >= 500.0f && shell_timer < 800.0f) {
                SetImage({RESOURCE_DIR"/Entities/shell0.png"}, 1000, 0);
            } else if (shell_timer >= 800.0f) {
                isShell = false;
                shell_timer = 0.0f;
                SetMoving(true);
                SetImage(AnimationRun, 500, 0);
                current_animation = AnimationState::Walking;
            }
        }
    } else {
        if (current_animation != AnimationState::Walking) {
            SetImage(AnimationRun, 500, 0);
            current_animation = AnimationState::Walking;
        }
    }
    m_Transform.scale = glm::vec2{isFacingRight ? -KOOPA_MAGNIFICATION : KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
}

bool FlyKoopa::AABBCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    return Koopa::AABBCollides(flykoopa_pos, box);
}

bool FlyKoopa::CCDDCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box) {
    return Koopa::CCDDCollides(flykoopa_pos, box);
}

bool FlyKoopa::DetectHole(float next_x, float y) {
    float check_y = y - (m_Drawable->GetSize().y * KOOPA_MAGNIFICATION / 2) - 5.0f;
    bool has_ground = false;
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
    float overlaps[4] = {koopa_right - mario_left, mario_right - koopa_left, koopa_top - mario_bottom, mario_top - koopa_bottom};
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
            ConvertToUnfly();
            mario->OnKillJump();
            mario->IncreaseScore(score);
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else if (!isShell) {
            ConvertToShell();
            mario->OnKillJump();
            mario->IncreaseScore(score);
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else if (isMovingShell) {
            isMovingShell = false;
            SetMoving(false);
            mario->OnKillJump();
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
        } else {
            ActivateShell(mario_pos.x < koopa_pos.x);
            mario->OnKillJump();
            sound = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        }
        if (sound) {
            sound->SetVolume(200);
            sound->Play();
        }
        return false;
    }
    if ((!isShell || (isShell && isMovingShell)) && !mario->is_temporarily_invincible) {
        if (mario->GetLive() > 0) mario->Die();
    } else if (isShell && !isMovingShell) {
        if (min_index == 0 || min_index == 1) {
            ActivateShell(mario_pos.x < koopa_pos.x);
            std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
            if (kick_sfx) {
                kick_sfx->SetVolume(200);
                kick_sfx->Play();
            }
            return true;
        }
    }
    return true;
}

void FlyKoopa::ConvertToUnfly() {
    if (isFlying) {
        isFlying = false;
        SetMoving(true);
        isFacingRight = false;
        SetImage(AnimationRun, 500, 0);
        current_animation = AnimationState::Walking;
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
        current_animation = AnimationState::Shell;
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
        current_animation = AnimationState::MovingShell;
        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        if (kick_sfx) {
            kick_sfx->SetVolume(200);
            kick_sfx->Play();
        }
    }
}

void FlyKoopa::SetLive(int live) {
    this->live = live;
    if (live == 0) {
        SetImage(AnimationRun, 1000, 0);
        current_animation = AnimationState::Walking;
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
    for (const auto& box : boxes) collision_boxes.push_back(box);
}

void FlyKoopa::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    collision_blocks.clear();
    for (const auto& block : blocks) collision_blocks.push_back(block);
}

void FlyKoopa::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void FlyKoopa::ClearCollisionBlocks() {
    collision_blocks.clear();
}