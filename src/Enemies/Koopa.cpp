#include "Enemies/Koopa.hpp"
#include "Global.hpp"
#include "Mario.hpp"
#include "App.hpp"
#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"

void Koopa::AddEnemies(std::vector<std::shared_ptr<Enemy>> enemies) {
    other_enemies.clear();
    for (const auto& enemy : enemies) {
        if (this != enemy.get()) other_enemies.push_back(enemy);
    }
}

void Koopa::ClearEnemies() {
    other_enemies.clear();
}

bool Koopa::CheckEnemyCollision(std::shared_ptr<Enemy> enemy) {
    if (this == enemy.get() || is_dead || !GetVisible() || enemy->GetIsDead()) return false;

    glm::vec2 this_pos = GetPosition();
    glm::vec2 this_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
    glm::vec2 enemy_pos = enemy->GetPosition();
    glm::vec2 enemy_size = enemy->GetSize() * ENEMY_MAGNIFICATION;

    float EPSILON = 0.01f;
    float this_left = this_pos.x - this_size.x / 2 + EPSILON;
    float this_right = this_pos.x + this_size.x / 2 - EPSILON;
    float this_top = this_pos.y + this_size.y / 2 - EPSILON;
    float this_bottom = this_pos.y - this_size.y / 2 + EPSILON;

    float enemy_left = enemy_pos.x - enemy_size.x / 2 + EPSILON;
    float enemy_right = enemy_pos.x + enemy_size.x / 2 - EPSILON;
    float enemy_top = enemy_pos.y + enemy_size.y / 2 - EPSILON;
    float enemy_bottom = enemy_pos.y - enemy_size.y / 2 + EPSILON;

    bool collision_x = (this_left < enemy_right) && (this_right > enemy_left);
    bool collision_y = (this_bottom < enemy_top) && (this_top > enemy_bottom);

    if (collision_x && collision_y) {
        if (is_shell && shell_is_moving) {
            KillEnemy(enemy);
            return true;
        }
        if (auto other_koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
            if (other_koopa->is_shell && other_koopa->shell_is_moving) {
                if (this->is_shell && this->shell_is_moving) {
                    BounceOffShell(other_koopa);
                } else {
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

    /**
     * 1. mario is not invincible AND koopa is shell
     *
     *
     *
     */
    if ((!is_shell || (is_shell && shell_is_moving) )&& mario->is_temporarily_invincible == false)  {
        if ( mario->GetLive() > 0) {
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

void Koopa::KillEnemy(std::shared_ptr<Enemy> enemy) {
    if (!enemy->GetVisible()) return;

    std::shared_ptr<Util::SFX> kick_sfx;

    if (auto goomba = std::dynamic_pointer_cast<Goomba>(enemy)) {
        if (!goomba->GetIsDead()) {
            goomba->SetDeadState(DeadState::Hit);
            goomba->SetLive(0);
            goomba->SetScale(GOOMBA_MAGNIFICATION, -GOOMBA_MAGNIFICATION);
            goomba->velocityY = 200.0f;
            kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        }
    } else if (auto koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
        if (koopa->is_shell && koopa->shell_is_moving) {
            BounceOffShell(koopa);
            return;
        }

        if (!koopa->GetIsDead()) {
            koopa->SetLive(0);
            koopa->SetScale(KOOPA_MAGNIFICATION, -KOOPA_MAGNIFICATION);
            koopa->velocityY = 200.0f;
            kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
        }
    }

    if (kick_sfx) {
        kick_sfx->SetVolume(200);
        kick_sfx->Play();
    }
}

void Koopa::BounceOffShell(std::shared_ptr<Koopa> other_koopa) {
    this->isFacingRight = !this->isFacingRight;
    other_koopa->isFacingRight = !other_koopa->isFacingRight;

    this->shell_timer = other_koopa->shell_timer = 0.0f;
    this->shell_is_moving = other_koopa->shell_is_moving = true;
    this->SetMoving(true);
    other_koopa->SetMoving(true);

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
    SetMoving(true);

    glm::vec2 mario_pos = mario->GetPosition();
    glm::vec2 koopa_pos = GetPosition();
    isFacingRight = (mario_pos.x < koopa_pos.x);

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

        if (GetOverworld()) {
            SetImage(AnimationDead, 1000, 0);
        } else {
            SetImage(AnimationUnderWorldDead, 1000, 0);
        }

        SetMoving(false);
        velocityY = 0.0f;

        std::shared_ptr<Util::SFX> shell_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.mp3");
        if (shell_sfx) {
            shell_sfx->SetVolume(200);
            shell_sfx->Play();
        }
    }
}

void Koopa::Action(const float distance) {
    if (is_shell && !shell_is_moving) return;

    float Koopa_x = GetPosition().x;
    float Koopa_y = GetPosition().y;
    glm::vec2 Koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;

    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;
    float step_distance = std::min(step, std::abs(distance));

    if (!isFacingRight) step_distance *= -1;

    bool collision = false;
    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ?
            std::min(step, remaining_distance) : std::max(-step, remaining_distance);
        float next_x = Koopa_x + step_distance;

        for (const auto& box : collision_boxes) {
            if (!box->GetVisible()) continue;
            AABBCollides({next_x, Koopa_y}, box);
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }
        if (collision) break;

        for (const auto& block : collision_blocks) {
            if (!block->GetVisible()) continue;
            AABBCollides({next_x, Koopa_y}, block);
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }
        if (collision) break;

        for (const auto& enemy : other_enemies) {
            if (this == enemy.get() || !enemy->GetVisible()) continue;

            if (auto goomba = std::dynamic_pointer_cast<Goomba>(enemy)) {
                if (goomba->GetIsDead()) continue;
            }
            if (auto koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
                if (koopa->GetIsDead()) continue;
            }

            glm::vec2 original_pos = GetPosition();
            SetPosition(next_x, Koopa_y);
            bool would_collide = CheckEnemyCollision(enemy);
            SetPosition(original_pos.x, original_pos.y);

            if (would_collide && !(is_shell && shell_is_moving)) {
                collision = true;
                break;
            }
        }
        if (collision) break;

        Koopa_x = next_x;
        SetPosition(Koopa_x, Koopa_y);
        remaining_distance -= step_distance;
    }

    if (collision) isFacingRight = !isFacingRight;
}

bool Koopa::AABBCollides(glm::vec2 Koopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Koopa_pos;
    glm::vec2 Koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
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

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) return false;

    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return X_state != CollisionState::None;
}

bool Koopa::CCDDCollides(glm::vec2 Koopa_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = Koopa_pos;
    glm::vec2 Koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
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

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) return false;

    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
}

bool Koopa::GravityAndCollision(const float delta) {
    glm::vec2 Koopa_size = m_Drawable->GetSize() * KOOPA_MAGNIFICATION;
    float Koopa_x = GetPosition().x;
    float Koopa_y = GetPosition().y;

    velocityY += GRAVITY * (delta / 60.0f);
    Koopa_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes) {
        if (!box->GetVisible()) continue;

        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CCDDCollides({Koopa_x, Koopa_y}, box);

        if (Y_state == CollisionState::Bottom) {
            Koopa_y = box->GetTransform().translation.y + b_size.y / 2 + Koopa_size.y / 2;
            velocityY = 0;
            SetPosition(Koopa_x, Koopa_y);
            return false;
        }
        if (Y_state == CollisionState::Top) {
            Koopa_y = box->GetTransform().translation.y - b_size.y / 2 - Koopa_size.y / 2;
            SetPosition(Koopa_x, Koopa_y);
            break;
        }
    }

    for (const auto &block : collision_blocks) {
        if (block->GetBroken()) continue;

        glm::vec2 b_size = block->GetSize();
        b_size.x *= block->GetScale().x;
        b_size.y *= block->GetScale().y;

        collision = CCDDCollides({Koopa_x, Koopa_y}, block);

        if (Y_state == CollisionState::Bottom) {
            Koopa_y = block->GetTransform().translation.y + b_size.y / 2 + Koopa_size.y / 2;
            velocityY = 0;
            SetPosition(Koopa_x, Koopa_y);
            return false;
        }
    }

    SetPosition(Koopa_x, Koopa_y);
    return !collision;
}

void Koopa::UpdateAnimation() {
    if (is_shell && !shell_is_moving) {
        shell_timer += delta_time;

        if (shell_timer >= 500.0f && shell_timer < 800.0f) {
            if (GetOverworld()) {
                SetImage({RESOURCE_DIR"/Entities/shell0.png"}, 1000, 0);
            } else {
                SetImage({RESOURCE_DIR"/Entities/Underworld/shell0.png"}, 1000, 0);
            }
        } else if (shell_timer >= 800.0f) {
            is_shell = false;
            shell_timer = 0.0f;
            SetMoving(true);

            if (GetOverworld()) {
                SetImage(AnimationRun, 500, 0);
            } else {
                SetImage(AnimationUnderWorldRun, 500, 0);
            }
            is_set_runanimation = true;
        }
    }

    m_Transform.scale = glm::vec2{isFacingRight ? -KOOPA_MAGNIFICATION : KOOPA_MAGNIFICATION, KOOPA_MAGNIFICATION};
}

void Koopa::OnUpdate(const float delta) {
    if (!GetMoving()) return;
    delta_time = delta;
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

    GravityAndCollision(3 * delta);
    UpdateAnimation();

    if (is_shell && !shell_is_moving) return;

    float distance = GetMoveVelocity() * delta;
    if (!isFacingRight) distance *= -1;
    if (is_shell) distance *= 3;

    Action(distance);
}

void Koopa::Move() {
    if (!GetMoving()) return;

    OnUpdate(1);

    if (!is_shell && is_set_runanimation == false) {
        if (GetOverworld()) {
            SetImage(AnimationRun, 500, 0);
        } else {
            SetImage(AnimationUnderWorldRun, 500, 0);
        }
        is_set_runanimation = true;
    }
}

void Koopa::SetLive(const int live) {
    this->live = live;
    is_dead = false;
    if (live == 0) {
        if (GetOverworld()) {
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