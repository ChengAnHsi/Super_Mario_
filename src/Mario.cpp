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
        state = MarioState::Jump;
        SetJumpAnimation();
        std::shared_ptr<Util::SFX> jump_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx->Play();
    }
}

void Mario::OnSmallJump() {
    if (is_growing) return;
    if (!isJumping) {
        velocityY = SMALL_JUMP_VELOCITY;
        isJumping = true;
        state = MarioState::Jump;
        SetJumpAnimation();
        std::shared_ptr<Util::SFX> jump_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx->Play();
    }
}

void Mario::OnKillJump() {
    if (is_growing) return;
    velocityY = SMALL_JUMP_VELOCITY;
    isJumping = true;
    state = MarioState::Jump;
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
            AABBCollides({next_x, mario_y}, box);
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
            AABBCollides({next_x, mario_y}, block);
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
            if (AABBCollides({next_x, mario_y}, collectible)) {
                IncreaseCoin(1);
                collectible->SetVisible(false);
            }
        }

        mario_x = next_x;
        this->SetPosition({ mario_x, mario_y });
        remaining_distance -= step_distance;
    }
}

bool Mario::AABBCollides(glm::vec2 mario_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = mario_pos;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    X_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // calculate minimum overlap area
    float minOverlap = std::min({bright - aleft, aright - bleft});

    if (minOverlap == bright - aleft) X_state = CollisionState::Left;
    else if (minOverlap == aright - bleft) X_state = CollisionState::Right;

    return X_state != CollisionState::None;
}

bool Mario::CCDDCollides(glm::vec2 mario_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = mario_pos;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();

    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    Y_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // calculate minimum overlap area
    float minOverlap = std::min({atop - bbottom, btop - abottom});

    if (minOverlap == atop - bbottom) Y_state = CollisionState::Top;
    else if (minOverlap == btop - abottom) Y_state = CollisionState::Bottom;

    return Y_state != CollisionState::None;
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

        collision = CCDDCollides({mario_x, mario_y}, box);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            mario_y = box->GetTransform().translation.y + b_size.y / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition({ mario_x, mario_y });
            return false;  // 碰撞到地面，不在滯空狀態
        }
        if(Y_state == CollisionState::Top) {
            // 固定在方塊下方開始下墜
            mario_y = box->GetTransform().translation.y - b_size.y / 2 - mario_size.y / 2;
            this->SetPosition({ mario_x, mario_y });
            break;
        }
    }
    // check collect collectible
    for (const auto& collectible : collision_collectibles) {
        // collectible had already been collected
        if (collectible->GetVisible() == false) {
            continue;
        }
        if (CCDDCollides({mario_x, mario_y}, collectible)) {
            IncreaseCoin(1);
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

        collision = CCDDCollides({mario_x, mario_y}, block);

        if (Y_state == CollisionState::Bottom) {
            // 固定瑪利歐在地板位置
            mario_y = block->GetTransform().translation.y + b_size.y / 2 + mario_size.y / 2;
            velocityY = 0;
            this->SetPosition({ mario_x, mario_y });
            return false;  // 碰撞到地面，不在滯空狀態
        }

        if(Y_state == CollisionState::Top) {
            block->TriggerJumpAnimation();

            if (block->GetBlockType() == Block::TYPE::MysteryBlock) {
                if (std::dynamic_pointer_cast<MysteryBlock>(block)->GetInsidePropType()[0] == Block::PROP_TYPE::Coin && block->GetCollisionTime() > 0) IncreaseCoin(1);
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
            this->SetPosition({ mario_x, mario_y });
            break;
        }
    }
    if (Y_state == CollisionState::Top) {
        velocityY = 0;

        if(collision_top_block) {
            // check collect collectible
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

    this->SetPosition({ mario_x, mario_y });

    // 如果沒有碰撞，表示在滯空狀態
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
        state = MarioState::Jump;
        return;
    }
    // Standing or running on the ground depending on whether you are moving or not
    if(is_down_key_down) {
        state = MarioState::Squat;
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
        if(state != MarioState::Run) {
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
        state = MarioState::Run;
        return;
    }
    if(state != MarioState::Stand) {
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
    state = MarioState::Stand;
}

void Mario::PullFlag() {
    if (GetPosition().y >= -360.0f + 3 * BLOCK_SIZE + GetSize().y * MARIO_MAGNIFICATION) {
        this->SetPosition({GetPosition().x, GetPosition().y - 1.0f});
    }else {
        this->SetPosition({GetPosition().x, GetPosition().y + BLOCK_SIZE / 2});
        m_Transform.scale = glm::vec2{-MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
        is_pull = false;
        is_back_to_castle = true;
    }
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

    float mario_x = this->GetPosition().x;
    float mario_y = this->GetPosition().y;

    float height_offset = (32.0f - 16.0f) / 2.0f * MARIO_MAGNIFICATION;
    this->SetPosition({mario_x, mario_y + height_offset});

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

    if(IfAnimationEnds()) {
        if(is_invincible) {
            if (is_fire) {
                this->SetImages(this->AnimationStandGrowFireInvincible, 100, 0);
                AnimationFireLast = AnimationStandGrowFireInvincible;
            }else {
                this->SetImages(this->AnimationStandGrowInvincible, 100, 0);
            }
        }else {
            if (is_fire) {
                this->SetImages(this->AnimationStandGrowFire, 100, 0);
                AnimationFireLast = AnimationStandGrowFire;
            }else {
                this->SetImages(this->AnimationStandGrow, 100, 0);
            }
        }
        this->SetLooping(true);
        is_growing = false;
    }
}

void Mario::Die() {
    if (is_dead || is_dying) return; // Already dead

    if (is_grow && is_growing == false) {
        // If Mario is grown, shrink him and play powerdown sound
        is_grow = false;
        is_fire = false;

        std::shared_ptr<Util::SFX> powerdown_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/powerdown.mp3");
        powerdown_sfx->SetVolume(70);
        powerdown_sfx->Play();

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

        velocityY += GRAVITY * (delta / 60.0f) * 3.0f;

        float mario_x = GetPosition().x;
        float mario_y = GetPosition().y + velocityY * (delta / 60.0f);
        this->SetPosition({mario_x, mario_y});
    }
}

float Mario::OnUpdate(const float delta) {
    // update moving
    int direction = is_right_key_down - is_left_key_down;
    if(is_down_key_down) direction = 0;
    const float distance = direction * velocityX * delta;

    OnRun(distance);

    isJumping = GravityAndCollision(3 * delta);

    UpdateAnimation();

    return distance;
}

void Mario::Fire() {
    if (shoot_fireball_timer != FIREBALL_SHOOT_TIME) return;
    shoot_fireball_timer = 0;
    SetImages({RESOURCE_DIR"/Entities/mario_fire_throw.png"}, 1000, 0);

    auto fball = std::make_shared<Fireball>();
    if (isFacingRight) {
        fball->SetPosition(GetPosition().x, GetPosition().y - BLOCK_SIZE / 4);
    }else {
        fball->SetPosition(GetPosition().x, GetPosition().y - BLOCK_SIZE / 4);
    }
    fball->AddCollisionBlocks(collision_blocks);
    fball->AddCollisionBoxes(collision_boxes);
    fball->SetFacingRight(isFacingRight);
    m_FM->AddFireball(fball);
}

void Mario::SetFireballManager(std::shared_ptr<FireballManager> FM) {
    this->m_FM = FM;
}

float Mario::Move() {
    if (is_dying) {
        // clear key down state
        is_left_key_down = false;
        is_right_key_down = false;
        UpdateDeadState(1.0f);
        return 0.0f; // No camera movement when dead
    }
    if (is_growing) {
        // clear key down state
        is_left_key_down = false;
        is_right_key_down = false;
        UpdateGrowingState();
        return 0.0f; // 阻止移動與其他輸入處理
    }
    if (is_pull) {
        // clear key down state
        is_left_key_down = false;
        is_right_key_down = false;
        PullFlag();
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
        }
    }
    if (!is_back_to_castle) {
        is_left_key_down = false;
        is_right_key_down = false;
    }
    if (shoot_fireball_timer < FIREBALL_SHOOT_TIME) {
        if (shoot_fireball_timer == 10) SetImages(AnimationFireLast, 1000, 0);
        shoot_fireball_timer += delta_time;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN) && is_grow && !is_back_to_castle) {
        is_down_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        is_left_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        is_right_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::UP) && !is_back_to_castle) {
        OnJump();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) && !is_back_to_castle) {
        OnSmallJump();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LSHIFT) || Util::Input::IsKeyPressed(Util::Keycode::RSHIFT) && !is_back_to_castle) {
        if (is_fire) Fire();
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
    // test locate to center
    if (Util::Input::IsKeyDown(Util::Keycode::A)) SetPosition({-20.0f, 0.0f});

    return OnUpdate(1);
}

void Mario::SetGrow(bool is_grow) {
    this->is_grow = is_grow;
}

bool Mario::GetGrow() {
    return is_grow;
}

bool Mario::GetGrowing() {
    return is_growing;
}

void Mario::SetFire(bool is_fire) {
    this->is_fire = is_fire;
}

void Mario::SetPull(bool is_pull) {
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

bool Mario::GetPull() {
    return is_pull;
}

void Mario::SetInvincible(bool is_invincible) {
    this->is_invincible = is_invincible;
    if(is_invincible) {
        invincible_timer = 0.0f;
        // todo update BGM
    }
}

bool Mario::GetInvincible() {
    return is_invincible;
}

bool Mario::GetFire() {
    return is_fire;
}

bool Mario::GetBackToCastle() {
    return is_back_to_castle;
}

bool Mario::GetReadyNextPhase() {
    return is_ready_for_next_phase;
}

void Mario::ResetStateForNextPhase() {
    SetVisible(true);
    is_pull = false;
    is_back_to_castle = false;
    is_ready_for_next_phase = false;
}

void Mario::IncreaseCoin(const int coin) {
    this->coin += coin;
    std::shared_ptr<Util::SFX> coin_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/coin.mp3");
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

void Mario::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Mario::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Mario::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Mario::ClearCollisionBlocks() {
    collision_blocks.clear();
}

void Mario::AddCollectibles(std::vector<std::shared_ptr<BackgroundImage>> collectibles) {
    for (const auto& collectible : collectibles) {
        collision_collectibles.push_back(collectible);
    }
}

void Mario::ClearCollectibles() {
    collision_collectibles.clear();
}