#include "Enemies/Koopa.hpp"
#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"
void Koopa::AddEnemies(std::vector<std::shared_ptr<Enemy>> enemies) {
    other_enemies.clear();
    for (const auto& enemy : enemies) {
        // Don't add self to the list
        if (this != enemy.get()) {
            other_enemies.push_back(enemy);
        }
    }
}

void Koopa::ClearEnemies() {
    other_enemies.clear();
}

bool Koopa::CheckEnemyCollision(std::shared_ptr<Enemy> enemy) {
    // Don't check collision with self or if either enemy is dead or not visible
    if (this == enemy.get() || is_dead || !GetVisible()) {
        return false;
    }

    // Check if the other enemy is dead
    bool other_enemy_dead = enemy->GetIsDead();

    if (other_enemy_dead) return false;

    // Get positions and sizes
    glm::vec2 this_pos = GetPosition();
    glm::vec2 this_size = m_Drawable->GetSize();
    this_size *= KOOPA_MAGNIFICATION;

    glm::vec2 enemy_pos = enemy->GetPosition();
    glm::vec2 enemy_size = enemy->GetSize();

    enemy_size *= ENEMY_MAGNIFICATION;

    // Calculate bounding boxes with smaller EPSILON for more precise detection
    float EPSILON = 0.01f;
    float this_left = this_pos.x - this_size.x / 2 + EPSILON;
    float this_right = this_pos.x + this_size.x / 2 - EPSILON;
    float this_top = this_pos.y + this_size.y / 2 - EPSILON;
    float this_bottom = this_pos.y - this_size.y / 2 + EPSILON;

    float enemy_left = enemy_pos.x - enemy_size.x / 2 + EPSILON;
    float enemy_right = enemy_pos.x + enemy_size.x / 2 - EPSILON;
    float enemy_top = enemy_pos.y + enemy_size.y / 2 - EPSILON;
    float enemy_bottom = enemy_pos.y - enemy_size.y / 2 + EPSILON;

    // Check for collision
    bool collision_x = (this_left < enemy_right) && (this_right > enemy_left);
    bool collision_y = (this_bottom < enemy_top) && (this_top > enemy_bottom);

    if (collision_x && collision_y) {
        // Collision handling based on state
        if (is_shell && shell_is_moving) {
            // Shell is moving - should kill other enemies
            KillEnemy(enemy);
            return true;
        }
        if (auto other_koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
            // Handle Koopa-Koopa collision
            if (other_koopa->is_shell && other_koopa->shell_is_moving) {
                if (this->is_shell && this->shell_is_moving) {
                    // Both are moving shells - should bounce off each other
                    BounceOffShell(other_koopa);
                } else {
                    // This koopa gets killed by the moving shell
                    this->SetLive(0);
                    this->SetScale(KOOPA_MAGNIFICATION, -KOOPA_MAGNIFICATION);
                    this->velocityY = 200.0f;
                }
                return true;
            }
        }
        return true;
    }

    return false;
}
bool Koopa::CheckMarioCollision(std::shared_ptr<Mario> mario) {
    if (is_dead || !GetVisible() || mario->is_dying) {
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
    float EPSILON = 1.0f;  // Small error tolerance for floating point comparison
    bool collision_x = (mario_left < koopa_right - EPSILON) && (mario_right > koopa_left + EPSILON);
    bool collision_y = (mario_bottom < koopa_top - EPSILON) && (mario_top > koopa_bottom + EPSILON);

    if (!(collision_x && collision_y)) return false;

    // todo kill koopa
    if(mario->GetInvincible()) {
        SetLive(0);
        dead_state = DeadState::Hit;
        // Apply death animation - flip upside down
        SetScale(ENEMY_MAGNIFICATION, -ENEMY_MAGNIFICATION);
        death_timer = 0.0f;
        velocityY = DEATH_JUMP_VELOCITY * 1.5f;

        std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        kick_sfx->SetVolume(200);
        kick_sfx->Play();

        mario->IncreaseScore(score);
        return true;
    }

    // Determine collision side by finding minimum overlap
    float overlaps[4];
    overlaps[0] = koopa_right - mario_left;   // Mario collides from left side
    overlaps[1] = mario_right - koopa_left;   // Mario collides from right side
    overlaps[2] = koopa_top - mario_bottom;   // Mario collides from bottom (stomp)
    overlaps[3] = mario_top - koopa_bottom;   // Mario collides from top

    // Find minimum overlap
    int min_index = 0;
    float min_overlap = overlaps[0];
    for (int i = 1; i < 4; i++) {
        if (overlaps[i] < min_overlap) {
            min_overlap = overlaps[i];
            min_index = i;
        }
    }

    // Handle collision based on the direction
    bool mario_moving_down = mario->velocityY <= 0;

    // Calculate horizontal overlap percentage for better accuracy
    float horizontal_overlap = std::min(mario_right, koopa_right) - std::max(mario_left, koopa_left);
    float horizontal_percentage = horizontal_overlap / std::min(koopa_size.x, mario_size.x);

    // TOP COLLISION (STOMP)
    if (min_index == 2 && mario_moving_down && horizontal_percentage > 0.4f) {
        // Mario is stomping on Koopa from above
        if (!is_shell) {
            // First stomp: Turn Koopa into shell
            TurnToShell();
            // Make Mario bounce with a small jump
            mario->OnKillJump();
            // Increase Mario's score
            mario->IncreaseScore(score);

            // Play stomp sound effect
            std::shared_ptr<Util::SFX> stomp_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
            if (stomp_sfx) {
                stomp_sfx->SetVolume(200);
                stomp_sfx->Play();
            }
            return false;
        }
        // Koopa is already a shell
        if (shell_is_moving) {
            // Stop the moving shell
            shell_is_moving = false;
            SetMoving(false);
            mario->OnKillJump(); // Make Mario bounce

            // Play stop shell sound effect
            std::shared_ptr<Util::SFX> stop_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/stomp.wav");
            if (stop_sfx) {
                stop_sfx->SetVolume(200);
                stop_sfx->Play();
            }
        } else {
            // Kick the stationary shell when stomping on it
            KickShell(mario);
            mario->OnKillJump(); // Make Mario bounce
        }
        return false;
    }
    // SIDE OR BOTTOM COLLISION
    // Left or right side collision (min_index 0 or 1) or bottom collision (min_index 3)
    if (!is_shell || (is_shell && shell_is_moving)) {
        // Only hurt Mario if not invincible and not coming from above
        if (!mario->GetInvincible() && mario->GetLive() > 0) {
            mario->Die();
        }
    } else if (is_shell && !shell_is_moving) {
        // Special handling for side collision with stationary shell
        if (min_index == 0 || min_index == 1) {
            // Kick the stationary shell from the side
            return KickShell(mario);
        } if (min_index == 3) {
            // Bottom collision (Mario from top) with stationary shell - Mario doesn't get hurt
            // Just allow Mario to stand/walk on top of the shell
            return true;
        }
        if (!mario->is_dying && mario->GetLive() > 0) {
            // Mario shrunk after a collision
            if (mario->is_temporarily_invincible == false) {
                mario->Die(); // Call our new Die method instead
                return true;
            }
        }
    }
    return true;
}
void Koopa::KillEnemy(std::shared_ptr<Enemy> enemy) {
    // Check if the enemy is already dead or not visible
    if (!enemy->GetVisible()) {
        return;
    }

    // Handle different enemy types
    if (auto goomba = std::dynamic_pointer_cast<Goomba>(enemy)) {
        // Kill the Goomba if it's not already dead
        if (!goomba->GetIsDead()) {
            goomba->SetDeadState(DeadState::Hit);
            goomba->SetLive(0);

            // Apply death animation - flip upside down
            goomba->SetScale(GOOMBA_MAGNIFICATION, -GOOMBA_MAGNIFICATION);

            // Apply upward velocity for "death jump"
            goomba->velocityY = 200.0f;

            // Play sound effect for killing enemy
            std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
            if (kick_sfx) {
                kick_sfx->SetVolume(200);
                kick_sfx->Play();
            }
        }
    }
    else if (auto koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
        // Don't kill a shell that's already moving
        if (koopa->is_shell && koopa->shell_is_moving) {
            // Instead, handle shell vs shell collision
            BounceOffShell(koopa);
            return;
        }

        // Kill the Koopa if it's not already dead
        if (!koopa->GetIsDead()) {
            koopa->SetLive(0);

            // Apply death animation - flip upside down
            koopa->SetScale(KOOPA_MAGNIFICATION, -KOOPA_MAGNIFICATION);

            // Apply upward velocity for "death jump"
            koopa->velocityY = 200.0f;

            // Play sound effect for killing enemy
            std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
            if (kick_sfx) {
                kick_sfx->SetVolume(200);
                kick_sfx->Play();
            }
        }
    }
}
void Koopa::BounceOffShell(std::shared_ptr<Koopa> other_koopa) {
    // Reverse direction for both shells
    this->isFacingRight = !this->isFacingRight;
    other_koopa->isFacingRight = !other_koopa->isFacingRight;

    // Reset shell timer to keep shells moving
    this->shell_timer = 0.0f;
    other_koopa->shell_timer = 0.0f;

    // Ensure both shells are still moving
    this->shell_is_moving = true;
    other_koopa->shell_is_moving = true;

    // Make sure movement is enabled
    this->SetMoving(true);
    other_koopa->SetMoving(true);

    // Play collision sound effect
    std::shared_ptr<Util::SFX> bounce_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/bump.wav");
    if (bounce_sfx) {
        bounce_sfx->SetVolume(200);
        bounce_sfx->Play();
    }
}
bool Koopa::KickShell(std::shared_ptr<Mario> mario) {
    if (!is_shell) return false;

    shell_is_moving = true;
    shell_timer = 0.0f;
    SetMoving(true); // Ensure the shell starts moving

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

        // Stop the movement
        SetMoving(false);

        // Reset velocity to prevent floating
        velocityY = 0.0f;

        // Play sound effect
        std::shared_ptr<Util::SFX> shell_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.mp3");
        if (shell_sfx) {
            shell_sfx->SetVolume(200);
            shell_sfx->Play();
        }
    }
}


void Koopa::Action(const float distance) {
    // If in shell mode and not moving, don't perform any movement
    if (is_shell && !shell_is_moving) {
        return;
    }

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
        float next_x = Koopa_x + step_distance;  // Calculate next position

        // Check wall and block collisions (existing code)
        for (const auto& box : collision_boxes) {
            // Skip invisible boxes
            if (box->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, Koopa_y}, box);
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }

        if (collision) {
            break;
        }

        for (const auto& block : collision_blocks) {
            // Skip invisible blocks
            if (block->GetVisible() == false) {
                continue;
            }
            AABBCollides({next_x, Koopa_y}, block);
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }

        if (collision) {
            break;
        }

        // Check enemy collisions
        for (const auto& enemy : other_enemies) {
            // Skip self or invisible/dead enemies
            if (this == enemy.get() || !enemy->GetVisible()) {
                continue;
            }

            // If enemy is a Goomba and it's dead, skip collision check
            if (auto goomba = std::dynamic_pointer_cast<Goomba>(enemy)) {
                if (goomba->GetIsDead()) {
                    continue;
                }
            }

            // If enemy is a Koopa and it's dead, skip collision check
            if (auto koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
                if (koopa->GetIsDead()) {
                    continue;
                }
            }

            // Temporarily set position to check if next step would cause collision
            glm::vec2 original_pos = GetPosition();
            SetPosition(next_x, Koopa_y);

            bool would_collide = CheckEnemyCollision(enemy);

            // Reset position
            SetPosition(original_pos.x, original_pos.y);

            if (would_collide) {
                // No need to set collision flag if we're a moving shell
                if (!(is_shell && shell_is_moving)) {
                    collision = true;
                }
                break;
            }
        }

        if (collision) {
            break;
        }

        Koopa_x = next_x;
        this->SetPosition(Koopa_x, Koopa_y);
        remaining_distance -= step_distance;
    }

    if (collision) {
        isFacingRight = !isFacingRight;
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
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

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
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

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
    // If koopa is a shell and not moving, increment the shell timer
    if (is_shell && !shell_is_moving) {
        shell_timer += delta_time;

        // After a certain time (5 seconds), change shell appearance and prepare to revive
        if (shell_timer >= 500.0f && shell_timer < 800.0f) {
            // Change to shell0 (blinking shell) to indicate it's about to revive
            if (GetOverworld() == true) {
                SetImage({RESOURCE_DIR"/Entities/shell0.png"}, 1000, 0);
            } else {
                SetImage({RESOURCE_DIR"/Entities/Underworld/shell0.png"}, 1000, 0);
            }
        }
        // After 8 seconds, revive the Koopa
        else if (shell_timer >= 800.0f) {
            is_shell = false;
            shell_timer = 0.0f;
            SetMoving(true);

            // Reset to walking animation
            if (GetOverworld() == true) {
                SetImage(AnimationRun, 500, 0);
            } else {
                SetImage(AnimationUnderWorldRun, 500, 0);
            }
            is_set_runanimation = true;
        }
    }

    // Update the facing direction
    if (isFacingRight == false) {
        m_Transform.scale = glm::vec2{KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    } else {
        m_Transform.scale = glm::vec2{-KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
    }
}

void Koopa::OnUpdate(const float delta) {
    // Update delta_time for animation timing
    delta_time = delta;
    if(GetPosition().y >= -360.0f && dead_state == DeadState::Hit){
        // flying
        velocityY += GRAVITY * (delta / 60.0f) * 3.0f;

        float enemy_x = GetPosition().x;
        float enemy_y = GetPosition().y + velocityY * (delta / 60.0f);
        SetPosition(enemy_x, enemy_y);
        if (death_timer >= DEATH_ANIMATION_TIME) {
            // After the animation time, make the Goomba disappear
            SetVisible(false);
        }
        return;
    }

    // Apply gravity regardless of state to prevent floating
    GravityAndCollision(3 * delta);

    // Update animation (shell revival timing, sprite flipping)
    UpdateAnimation();

    // If koopa is in shell state but not moving, skip movement calculations
    if (is_shell && !shell_is_moving) {
        return;
    }

    float distance = GetMoveVelocity() * delta;

    // Adjust distance based on facing direction
    if (isFacingRight == false) {
        distance *= -1;
    }

    // Move the koopa/shell
    if (is_shell) distance *= 3;
    Action(distance);
}

void Koopa::Move(){
    if (!GetMoving()) return;

    OnUpdate(1);

    if (!is_shell && is_set_runanimation == false) {
        if (GetOverworld() == true) {
            SetImage(AnimationRun, 500, 0);
        } else {
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
        } else {
            SetImage(AnimationUnderWorldDead, 1000, 0);
        }
        is_dead = true;
        death_timer = 0.0f;
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