//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"

#include <iostream>

#include "Global.hpp"
#include "App.hpp"
#include "Blocks/MysteryBlock.hpp"

#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

void Mario::OnJump() {
    if (is_growing) return;
    if (!isJumping) {
        velocityY = JUMP_VELOCITY;
        isJumping = true;
        mario_state = MarioState::Jump;
        SetJumpAnimation();
        const auto jump_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx->Play();
    }
}

void Mario::OnSmallJump() {
    if (is_growing) return;
    if (!isJumping) {
        velocityY = SMALL_JUMP_VELOCITY;
        isJumping = true;
        mario_state = MarioState::Jump;
        SetJumpAnimation();
        const auto jump_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx->Play();
    }
}

void Mario::OnKillJump() {
    if (is_growing) return;
    velocityY = SMALL_JUMP_VELOCITY;
    isJumping = true;
    mario_state = MarioState::Jump;
    SetJumpAnimation();
}

void Mario::OnRun(const float distance) {
    if (distance == 0) return;
    if (is_back_to_castle) back_to_castle_dis += distance;

    float mario_x = GetPosition().x;
    float mario_y = GetPosition().y;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    // each step move block size / 4
    const float step = BLOCK_SIZE / 4.0f;
    float remaining_distance = distance;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        float next_x = mario_x + step_distance;  // 計算下一幀位置

        bool collision = false;
        for (const auto& box : collision_boxes) {
            // box had already destroyed
            if (box->GetVisible() == false) {
                continue;
            }
            CollidesAndSetDirection({next_x, mario_y}, box, true);
            // check next step will collision or not
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                collision = true;
                break;
            }
        }
        for (const auto& platform : collision_flyplatforms) {
            CollidesAndSetDirection({next_x, mario_y}, platform, true);
            // check next step will collision or not
            if (X_state == CollisionState::Left || X_state == CollisionState::Right) {
                CollidesAndSetDirection({next_x, mario_y}, platform, false);
                if (Y_state == CollisionState::Top) {
                    // ignore
                }
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
            CollidesAndSetDirection({next_x, mario_y}, block, true);
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

        // else do move and check collect collectible or not
        for (const auto& collectible : collision_collectibles) {
            // collectible had already been collected
            if (collectible->GetVisible() == false) {
                continue;
            }
            if (CollidesAndSetDirection({next_x, mario_y}, collectible, true)) {
                IncreaseCoin(1);
                IncreaseScore(200);
                collectible->SetVisible(false);
            }
        }

        mario_x = next_x;
        this->SetPosition(mario_x, mario_y);
        remaining_distance -= step_distance;
    }
}

bool Mario::CollidesAndSetDirection(const glm::vec2 mario_pos, const std::shared_ptr<BackgroundImage>& box, const bool is_checkX) {
    const glm::vec2 a = mario_pos;
    glm::vec2 a_size = this->m_Drawable->GetSize();
    a_size *= MARIO_MAGNIFICATION;

    const glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    if(is_checkX) X_state = CollisionState::None;
    else Y_state = CollisionState::None;

    const float A_left = a.x - a_size.x / 2;
    const float A_right = a.x + a_size.x / 2;
    const float A_top = a.y + a_size.y / 2;
    const float A_bottom = a.y - a_size.y / 2;

    const float B_left = b.x - b_size.x / 2;
    const float B_right = b.x + b_size.x / 2;
    const float B_top = b.y + b_size.y / 2;
    const float B_bottom = b.y - b_size.y / 2;

    const bool collisionX = A_left < B_right && A_right > B_left;
    const bool collisionY = A_bottom < B_top && A_top > B_bottom;

    if (!(collisionX && collisionY)) return false;

    // calculate minimum overlap area
    if(is_checkX) {
        // 判斷 X 軸碰撞方向
        const float overlapLeft = B_right - A_left;
        const float overlapRight = A_right - B_left;
        if (overlapLeft < overlapRight)
            X_state = CollisionState::Left;
        else
            X_state = CollisionState::Right;
    }else {
        // 判斷 Y 軸碰撞方向
        const float overlapTop = A_top - B_bottom;
        const float overlapBottom = B_top - A_bottom;
        if (overlapTop < overlapBottom)
            Y_state = CollisionState::Top;
        else
            Y_state = CollisionState::Bottom;
    }

    return true;
}

bool Mario::GravityAndCollision(const float delta) {
    if (is_growing) return isJumping;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= MARIO_MAGNIFICATION;
    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    // 更新垂直速度（根據重力）
    velocityY += GRAVITY * (delta / 60.0f);
    mario_y += velocityY * (delta / 60.0f);

    bool collision = false;
    for (const auto &box : collision_boxes) {
        // box had already destroyed
        if(box->GetVisible() == false) {
            continue;
        }
        glm::vec2 b_size = box->GetSize();
        b_size.x *= box->GetScale().x;
        b_size.y *= box->GetScale().y;

        collision = CollidesAndSetDirection({mario_x, mario_y}, box, false);

        if (Y_state == CollisionState::Bottom) {
            // fix mario on the floor
            mario_y = box->GetTransform().translation.y + b_size.y / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition(mario_x, mario_y);
            return false;  // on ground
        }
        if(Y_state == CollisionState::Top && velocityY > 0) {
            // 固定在方塊下方開始下墜
            mario_y = box->GetTransform().translation.y - b_size.y / 2 - mario_size.y / 2;
            this->SetPosition(mario_x, mario_y);
            velocityY = 0;
            break;
        }
    }
    // record Mario current standing on which platform
    std::shared_ptr<FlyPlatform> standing_platform = nullptr;
    bool is_on_platform = false;
    for (const auto& platform : collision_flyplatforms) {
        glm::vec2 b_size = platform->GetSize();
        b_size.x *= platform->GetScale().x;
        b_size.y *= platform->GetScale().y;

        collision = CollidesAndSetDirection({mario_x, mario_y}, platform, false);

        if (Y_state == CollisionState::Bottom) {
            standing_platform = platform;
            // fix mario on the floor
            mario_y = platform->GetTransform().translation.y + b_size.y / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition(mario_x, mario_y);
            is_on_platform = true;
            break;
        }
        if(Y_state == CollisionState::Top && velocityY > 0) {
            // 固定在方塊下方開始下墜
            mario_y = platform->GetTransform().translation.y - b_size.y / 2 - mario_size.y / 2;
            this->SetPosition(mario_x, mario_y);
            velocityY = 0;
            break;
        }
    }

    if (!is_right_key_down && !is_left_key_down && standing_platform) {
        float dx = standing_platform->GetDeltaX();
        float dy = standing_platform->GetDeltaY();
        SetPosition(GetPosition().x + dx, GetPosition().y + dy);
    }
    if (is_on_platform) return false;  // on ground

    // check collect collectible
    for (const auto& collectible : collision_collectibles) {
        // collectible had already been collected
        if (collectible->GetVisible() == false) {
            continue;
        }
        if (CollidesAndSetDirection({mario_x, mario_y}, collectible, false)) {
            IncreaseCoin(1);
            IncreaseScore(200);
            collectible->SetVisible(false);
        }
    }

    bool collision_top_block = false;
    glm::vec2 top_block_pos;

    for (const auto &block : collision_blocks) {
        // block had already destroyed
        if(block->GetBroken() == true) {
            continue;
        }
        glm::vec2 b_size = block->GetSize();
        b_size.x *= block->GetScale().x;
        b_size.y *= block->GetScale().y;

        collision = CollidesAndSetDirection({mario_x, mario_y}, block, false);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            mario_y = block->GetTransform().translation.y + b_size.y / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition(mario_x, mario_y);
            return false;  // 碰撞到地面，不在滯空狀態
        }

        if(Y_state == CollisionState::Top && velocityY > 0) {
            block->TriggerJumpAnimation();

            if (block->GetBlockType() == Block::TYPE::MysteryBlock) {
                if (std::dynamic_pointer_cast<MysteryBlock>(block)->GetInsidePropType()[0] == Block::PROP_TYPE::Coin && block->GetCollisionTime() > 0) {
                    IncreaseCoin(1);
                    IncreaseScore(200);
                }
                if (std::dynamic_pointer_cast<MysteryBlock>(block)->GetInsidePropType()[1] != Block::PROP_TYPE::None && is_grow) std::dynamic_pointer_cast<MysteryBlock>(block)->SetChooseProp(false);
            }

            if(is_grow || block->GetBlockType() != Block::TYPE::CommonBlock){
                block->AfterCollisionEvents();
            }

            if(block->GetBlockType() != Block::TYPE::ImmovableBlock) {
                collision_top_block = true;
                top_block_pos = block->GetPosition();
            }
            // 固定在方塊下方開始下墜
            mario_y = block->GetTransform().translation.y - b_size.y / 2 - mario_size.y / 2;
            this->SetPosition(mario_x, mario_y);
            velocityY = 0;
            break;
        }
    }
    if (Y_state == CollisionState::Top) {
        if(collision_top_block) {
            // check collect top position collectible
            for (const auto& collectible : collision_collectibles) {
                // collectible had already been collected
                if (collectible->GetVisible() == false) {
                    continue;
                }

                glm::vec2 a = {top_block_pos.x, top_block_pos.y + BLOCK_SIZE};
                glm::vec2 b = collectible->m_Transform.translation;
                glm::vec2 b_size = collectible->GetSize();
                b_size.x *= collectible->GetScale().x;
                b_size.y *= collectible->GetScale().y;

                float aleft = a.x - BLOCK_SIZE / 2;
                float aright = a.x + BLOCK_SIZE / 2;
                float atop = a.y + BLOCK_SIZE / 2;
                float abottom = a.y - BLOCK_SIZE / 2;

                float bleft = b.x - b_size.x / 2;
                float bright = b.x + b_size.x / 2;
                float btop = b.y + b_size.y / 2;
                float bbottom = b.y - b_size.y / 2;
                bool collisionX = (aleft < bright) && (aright > bleft);
                bool collisionY = (abottom < btop) && (atop > bbottom);
                if (collisionX && collisionY) {
                    IncreaseCoin(1);
                    collectible->SetVisible(false);
                }
            }
        }
        return true;
    }

    this->SetPosition(mario_x, mario_y);

    return !collision;
}

void Mario::UpdateAnimation() {
    if (is_growing) return;
    const int direction = is_right_key_down - is_left_key_down;
    // facing left
    if (direction == -1) {
        isFacingRight = false;
        m_Transform.scale = glm::vec2{-MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
    }
    // facing right
    if (direction == 1)  {
        isFacingRight = true;
        m_Transform.scale = glm::vec2{MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
    }

    if (isJumping) {
        // 會影響到跳躍後的站立的動畫，註解掉
        // if(state != MarioState::Jump) {
        //     this->SetImages(AnimationJump);
        // }
        mario_state = MarioState::Jump;
        return;
    }
    // Standing or running on the ground depending on whether you are moving or not
    if(is_down_key_down) {
        mario_state = MarioState::Squat;
        if(is_grow) {
            if(is_invincible){
                if (is_fire) {
                    SetImages(AnimationSquatGrowFireInvincible, 100, 0);
                    AnimationFireLast = AnimationSquatGrowFireInvincible;
                }else {
                    SetImages(AnimationSquatGrowInvincible, 100, 0);
                }
            }else {
                if (is_fire) {
                    SetImages(AnimationSquatGrowFire, 100, 0);
                    AnimationFireLast = AnimationSquatGrowFire;
                }else {
                    SetImages(AnimationSquatGrow, 100, 0);
                }
            }
        }
        return;
    }
    if (direction != 0) {
        if(mario_state != MarioState::Run) {
            this->SetPlaying(true);
            this->SetLooping(true);
            if(is_grow) {
                if(is_invincible) {
                    if (is_fire) {
                        this->SetImages(AnimationRunGrowFireInvincible, 25, 0);
                        AnimationFireLast = AnimationRunGrowFireInvincible;
                    }else {
                        this->SetImages(AnimationRunGrowInvincible, 25, 0);
                    }
                }else {
                    if (is_fire) {
                        this->SetImages(AnimationRunGrowFire, 100, 0);
                        AnimationFireLast = AnimationRunGrowFire;
                    }else {
                        this->SetImages(AnimationRunGrow, 100, 0);
                    }
                }
            }else {
                if(is_invincible) {
                    this->SetImages(AnimationRunInvincible, 25, 0);
                }else {
                    this->SetImages(AnimationRun, 100, 0);
                }
            }
        }
        isRunning = true;
        mario_state = MarioState::Run;
        return;
    }
    if(mario_state != MarioState::Stand) {
        this->SetPlaying(true);
        this->SetLooping(true);
        if (is_grow) {
            if(is_invincible) {
                if (is_fire) {
                    this->SetImages(AnimationStandGrowFireInvincible, 100, 0);
                    AnimationFireLast = AnimationStandGrowFireInvincible;
                }else {
                    this->SetImages(AnimationStandGrowInvincible, 100, 0);
                }
            }else {
                if (is_fire) {
                    this->SetImages(AnimationStandGrowFire, 100, 0);
                    AnimationFireLast = AnimationStandGrowFire;
                }else {
                    this->SetImages(AnimationStandGrow, 100, 0);
                }
            }
        }else {
            if(is_invincible) {
                this->SetImages(AnimationStandInvincible, 100, 0);
            }else {
                this->SetImages(AnimationStand, 100, 0);
            }
        }
    }
    isRunning = false;
    mario_state = MarioState::Stand;
}

void Mario::SetJumpAnimation() {
    if(is_grow) {
        if(is_invincible) {
            if (is_fire) {
                this->SetImages(AnimationJumpGrowFireInvincible, 100, 0);
                AnimationFireLast = AnimationJumpGrowFireInvincible;
            }else {
                this->SetImages(AnimationJumpGrowInvincible, 100, 0);
            }
        }else {
            if (is_fire) {
                this->SetImages(AnimationJumpGrowFire, 100, 0);
                AnimationFireLast = AnimationJumpGrowFire;
            }else {
                this->SetImages(AnimationJumpGrow, 100, 0);
            }
        }
    }else {
        if(is_invincible) {
            this->SetImages(AnimationJumpInvincible, 100, 0);
        }else {
            this->SetImages(AnimationJump, 100, 0);
        }
    }
}

void Mario::SetGrowingAnimation() {
    if (is_grow) return;

    is_grow = true;
    is_growing = true;
    growing_timer = 0.0f;

    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    float height_offset = (32.0f - 16.0f) / 2.0f * MARIO_MAGNIFICATION;
    this->SetPosition(mario_x, mario_y + height_offset);

    if(is_invincible) {
        if (is_fire) {
            this->SetImages(this->AnimationGrowFireInvincible, 200, 0);
            AnimationFireLast = AnimationGrowFireInvincible;
        }else {
            this->SetImages(this->AnimationGrowInvincible, 200, 0);
        }
    }else {
        this->SetImages(this->AnimationGrow, 200, 0);
    }
    this->SetLooping(false);

    std::shared_ptr<Util::SFX> grow_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Temp/Sound/mushroomeat.wav");
    grow_sfx->SetVolume(70);
    grow_sfx->Play();
}

void Mario::UpdateGrowingState() {
    if (is_grow == false) return;
    growing_timer += 1.0f;
    if(growing_timer >= GROWING_TIME) {
        growing_timer = 0.0f;
        // set animation at the first time
        if(isJumping) {
            if(is_fire && is_invincible) this->SetImages(AnimationJumpGrowFireInvincible, 100, 0);
            else if(is_fire) this->SetImages(AnimationJumpGrowFire, 100, 0);
            else if(is_invincible) this->SetImages(AnimationJumpGrowInvincible, 100, 0);
            else this->SetImages(AnimationJumpGrow, 100, 0);
        }else if(isRunning) {
            if(is_fire && is_invincible) this->SetImages(AnimationRunGrowFireInvincible, 100, 0);
            else if(is_fire) this->SetImages(AnimationRunGrowFire, 100, 0);
            else if(is_invincible) this->SetImages(AnimationRunGrowInvincible, 100, 0);
            else this->SetImages(AnimationRunGrow, 100, 0);
        }else {
            if(is_fire && is_invincible) this->SetImages(AnimationStandGrowFireInvincible, 100, 0);
            else if(is_fire) this->SetImages(AnimationStandGrowFire, 100, 0);
            else if(is_invincible) this->SetImages(AnimationStandGrowInvincible, 100, 0);
            else this->SetImages(AnimationStandGrow, 100, 0);
        }
        this->SetLooping(true);
        is_growing = false;
    }
}

void Mario::PullFlag() {
    if (GetPosition().y >= -360.0f + 3 * BLOCK_SIZE + GetSize().y * MARIO_MAGNIFICATION) {
        this->SetPosition(GetPosition().x, GetPosition().y - 1.0f);
    }else {
        this->SetPosition(GetPosition().x, GetPosition().y + BLOCK_SIZE / 2);
        m_Transform.scale = glm::vec2{-MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
        is_pull = false;
        is_back_to_castle = true;
    }
}

void Mario::DrillTube() {
    if(drill_tube_dis != 0 && drill_state != DrillState::None) drill_tube_dis -= 1.0f;
    switch (drill_state){
        case DrillState::Up:{
            this->SetPosition(GetPosition().x, GetPosition().y + 1.0f);
            break;
        }
        case DrillState::Down:{
            this->SetPosition(GetPosition().x, GetPosition().y - 1.0f);
            break;
        }
        case DrillState::Left:{
            m_Transform.scale = glm::vec2{-MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
            this->SetPosition(GetPosition().x - 1.0f, GetPosition().y);
            break;
        }
        case DrillState::Right:{
            m_Transform.scale = glm::vec2{MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
            this->SetPosition(GetPosition().x + 1.0f, GetPosition().y);
            break;
        }
        default:{
            break;
        }
    }
    if(drill_tube_dis == 0.0f) {
        if(drill_state == DrillState::Right) is_time_to_move_camera_map2 = true;
        drill_state = DrillState::None;
        is_drill = false;
        // just for map 1-2
    }
}

void Mario::Die() {
    if (is_dead || is_dying) return; // Already dead

    if (is_grow && is_growing == false) {
        // If Mario is grown, shrink him and play powerdown sound
        is_grow = false;
        is_fire = false;

        const auto power_down_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/powerdown.mp3");
        power_down_sfx->SetVolume(70);
        power_down_sfx->Play();

        this->SetImages(this->AnimationStand, 100, 0);

        is_temporarily_invincible = true;
        invincible_timer = 0.0f;
    } else if (is_growing == false){
        // Mario dies
        is_dying = true;
        is_fire = false;
        death_timer = 0.0f;

        // Play death sound
        std::shared_ptr<Util::SFX> death_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Temp/Sound/death.wav");
        death_sfx->SetVolume(70);
        death_sfx->Play();

        // Set death animation
        this->SetImages(AnimationDead, 100, 0);
    }
}

void Mario::UpdateDeadState(float delta) {
    if (!is_dying) return;

    death_timer += delta;

    if (death_timer <= DEATH_PAUSE_TIME) {
        // Just wait during the pause time - freeze position
        return;
    }

    if(GetPosition().y >= -360.0f){
        // flying
        if (death_timer == DEATH_PAUSE_TIME + 1) {
            // setting death jump velocity after freeze
            velocityY = DEATH_JUMP_VELOCITY * 1.5f;
        }

        velocityY += DEATH_GRAVITY * (delta / 60.0f) * 3.0f;

        float mario_x = GetPosition().x;
        float mario_y = GetPosition().y + velocityY * (delta / 60.0f);
        this->SetPosition(mario_x, mario_y);
    }
}

float Mario::OnUpdate(const float delta) {
    // update moving
    int direction = is_right_key_down - is_left_key_down;
    if(is_down_key_down) direction = 0;

    const float distance = direction * velocityX * delta;

    if (is_run_key_down) OnRun(distance * 1.35);
    else OnRun(distance);

    isJumping = GravityAndCollision(3 * delta);

    UpdateAnimation();

    if (is_run_key_down) return distance * 1.35;
    return distance;

}

void Mario::Fire() {
    if (shoot_fireball_timer != FIREBALL_SHOOT_TIME) return;
    shoot_fireball_timer = 0;
    SetImages({RESOURCE_DIR"/Entities/mario_fire_throw.png"}, 100, 0);

    auto fball = std::make_shared<Fireball>();
    fball->SetPosition(GetPosition().x, GetPosition().y - BLOCK_SIZE / 4);
    fball->AddCollisionBlocks(collision_blocks);
    fball->AddCollisionBoxes(collision_boxes);
    fball->AddCollisionPlatforms(collision_flyplatforms);
    fball->SetFacingRight(isFacingRight);
    m_FM->AddFireball(fball);
}

float Mario::Move() {
    if (is_dead) {
        SetPosition(-380.0f + 2.5f * BLOCK_SIZE, -232.0f);
        return 0.0f;
    }
    if (is_dying) {
        // clear key down state
        is_left_key_down = false;
        is_right_key_down = false;
        is_run_key_down = false;
        UpdateDeadState(1.0f);
        return 0.0f;
    }
    if (is_growing) {
        is_left_key_down = false;
        is_right_key_down = false;
        is_run_key_down = false;
        UpdateGrowingState();
        return 0.0f;
    }
    if (is_pull) {
        is_left_key_down = false;
        is_right_key_down = false;
        is_run_key_down = false;
        PullFlag();
        return 0.0f;
    }
    if(is_drill) {
        is_left_key_down = false;
        is_right_key_down = false;
        DrillTube();
        if(drill_state == DrillState::Right) return 1.0f;
        return 0.0f;
    }
    if (is_temporarily_invincible) {
        invincible_timer += delta_time;
        // 3sec
        if (invincible_timer >= TEMP_INVINCIBLE_END_TIME) {
            is_temporarily_invincible = false;
        }
    }
    if (is_invincible) {
        invincible_timer += delta_time;
        // 30sec
        if (invincible_timer >= INVINCIBLE_END_TIME) {
            is_invincible = false;
            // set animation at the first time
            if(isJumping) {
                if(is_fire) this->SetImages(AnimationJumpGrowFire, 100, 0);
                else if(is_grow) this->SetImages(AnimationJumpGrow, 100, 0);
                else this->SetImages(AnimationJump, 100, 0);
            }else if(isRunning) {
                if(is_fire) this->SetImages(AnimationRunGrowFire, 100, 0);
                else if(is_grow) this->SetImages(AnimationRunGrow, 100, 0);
                else this->SetImages(AnimationRun, 100, 0);
            }else {
                if(is_fire) this->SetImages(AnimationStandGrowFire, 100, 0);
                else if(is_grow) this->SetImages(AnimationStandGrow, 100, 0);
                else this->SetImages(AnimationStand, 100, 0);
            }
        }
    }
    if (!is_back_to_castle) {
        is_left_key_down = false;
        is_right_key_down = false;
        is_run_key_down = false;
    }
    if (is_ready_for_next_phase) {
        is_left_key_down = false;
        is_right_key_down = false;
        is_run_key_down = false;
        return 0.0f;
    }

    if (shoot_fireball_timer < FIREBALL_SHOOT_TIME) {
        if (shoot_fireball_timer == 10) SetImages(AnimationFireLast, 1000, 0);
        shoot_fireball_timer += delta_time;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN) && is_grow && !is_back_to_castle) {
        is_down_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)&& !is_back_to_castle) {
        is_left_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)&& !is_back_to_castle) {
        is_right_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::UP) && !is_back_to_castle) {
        OnJump();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) && !is_back_to_castle) {
        OnSmallJump();
    }
    if ((Util::Input::IsKeyPressed(Util::Keycode::LSHIFT) || Util::Input::IsKeyPressed(Util::Keycode::RSHIFT)) && !is_back_to_castle) {
        if (is_fire) Fire();
        is_run_key_down = true;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::LSHIFT)||Util::Input::IsKeyUp(Util::Keycode::RSHIFT)){
       is_run_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) {
        is_left_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) {
        is_right_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::DOWN)) {
        is_down_key_down = false;
    }
    if (is_back_to_castle){
        // from flag to castle distance is 6 block size
        if (back_to_castle_dis <= 6 * BLOCK_SIZE) {
            is_right_key_down = true;
        }else {
            is_back_to_castle = false;
            is_right_key_down = false;
            is_ready_for_next_phase = true;
            SetVisible(false);
        }
    }

    if (Util::Input::IsKeyDown(Util::Keycode::A)) SetInvincible(true);

    return OnUpdate(1);
}

void Mario::SetGrow(bool is_grow) {
    this->is_grow = is_grow;
}

bool Mario::GetGrow() const {
    return is_grow;
}

bool Mario::GetGrowing() const {
    return is_growing;
}

void Mario::SetFire(const bool is_fire) {
    this->is_fire = is_fire;
}

void Mario::SetDead(const bool is_dead) {
    this->is_dead = is_dead;
}

bool Mario::GetDead() const {
    return is_dead;
}

void Mario::SetDying(const bool is_dying) {
    this->is_dying = is_dying;
}

bool Mario::GetDying() const {
    return is_dying;
}

void Mario::SetPull(const bool is_pull) {
    if (!is_pull) return;
    if (!this->is_pull) {
        if (is_grow) {
            SetImages(AnimationPullGrow, 500, 0);
        }else {
            SetImages(AnimationPull, 500, 0);
        }
    }
    this->is_pull = is_pull;
}

bool Mario::GetPull() const {
    return is_pull;
}

void Mario::SetDrill(const bool is_drill) {
    this->is_drill = is_drill;
}

bool Mario::GetDrill() const {
    return is_drill;
}

void Mario::SetDrillState(const DrillState drill_state) {
    this->drill_state = drill_state;
}

void Mario::SetDrillDistance(const float drill_tube_dis) {
    this->drill_tube_dis = drill_tube_dis;
}

void Mario::SetInvincible(const bool is_invincible) {
    this->is_invincible = is_invincible;
    if(is_invincible) {
        invincible_timer = 0.0f;
        // todo update BGM
        // set animation at the first time
        if(isJumping) {
            if(is_grow && is_fire) this->SetImages(AnimationJumpGrowFireInvincible, 100, 0);
            else if(is_grow) this->SetImages(AnimationJumpGrowInvincible, 100, 0);
            else this->SetImages(AnimationJumpInvincible, 100, 0);
        }else if(isRunning) {
            if(is_grow && is_fire) this->SetImages(AnimationRunGrowFireInvincible, 100, 0);
            else if(is_grow) this->SetImages(AnimationRunGrowInvincible, 100, 0);
            else this->SetImages(AnimationRunInvincible, 100, 0);
        }else {
            if(is_grow && is_fire) this->SetImages(AnimationStandGrowFireInvincible, 100, 0);
            else if(is_grow) this->SetImages(AnimationStandGrowInvincible, 100, 0);
            else this->SetImages(AnimationStandInvincible, 100, 0);
        }
    }
}

bool Mario::GetInvincible() const {
    return is_invincible;
}

bool Mario::GetTempInvincible() const {
    return is_temporarily_invincible;
}

bool Mario::GetFire() const {
    return is_fire;
}

void Mario::SetFireballManager(const std::shared_ptr<FireballManager> &FM) {
    this->m_FM = FM;
}

bool Mario::GetBackToCastle() const {
    return is_back_to_castle;
}

bool Mario::GetReadyNextPhase() const {
    return is_ready_for_next_phase;
}

bool Mario::GetTimeToMoveCamera() const {
    return is_time_to_move_camera_map2;
}

void Mario::SetTimeToMoveCamera(const bool is_time_to_move_camera_map2) {
    this->is_time_to_move_camera_map2 = is_time_to_move_camera_map2;
}

void Mario::ResetStateForNextPhase() {
    SetInvincible(false);
    SetVisible(true);
    back_to_castle_dis = 0.0f;
    is_pull = false;
    is_back_to_castle = false;
    is_ready_for_next_phase = false;
}

float Mario::GetVelocityY() const {
    return velocityY;
}

void Mario::IncreaseCoin(const int coin) {
    this->coin += coin;
    const auto coin_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/coin.mp3");
    coin_sfx->SetVolume(70);
    coin_sfx->Play();
}

int Mario::GetCoin() const {
    return coin;
}

void Mario::SetLive(const int live) {
    this->live = live;
}

int Mario::GetLive() const {
    return live;
}

void Mario::IncreaseScore(const int score) {
    this->score += score;
}

int Mario::GetScore() const {
    return score;
}

void Mario::AddCollisionBoxes(const std::vector<std::shared_ptr<BackgroundImage>>& boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Mario::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Mario::AddCollisionBlocks(const std::vector<std::shared_ptr<Block>>& blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Mario::ClearCollisionBlocks() {
    collision_blocks.clear();
}

void Mario::AddCollisionFlyPlatForms(const std::vector<std::shared_ptr<FlyPlatform>>& platforms) {
    for (const auto& platform : platforms) {
        collision_flyplatforms.push_back(platform);
    }
}

void Mario::ClearFlyPlatForms() {
    collision_flyplatforms.clear();
}

void Mario::AddCollectibles(const std::vector<std::shared_ptr<BackgroundImage>>& collectibles) {
    for (const auto& collectible : collectibles) {
        collision_collectibles.push_back(collectible);
    }
}

void Mario::ClearCollectibles() {
    collision_collectibles.clear();
}