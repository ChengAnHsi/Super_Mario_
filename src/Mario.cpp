//
// Created by 112590007 on 2025/3/6.
//

#include "Mario.hpp"

#include "BlockManager.hpp"
#include "Global.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "App.hpp"
void Mario::OnJump() {
    if (!isJumping) {
        velocityY = JUMP_VELOCITY;
        state = MarioState::Jump;
        if(is_grow) {
            this->SetImages(AnimationJumpGrow, 100, 0);
        }else {
            this->SetImages(AnimationJump, 100, 0);
        }
        std::shared_ptr<Util::SFX> jump_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx ->Play();
    }
}

void Mario::OnSmallJump() {
    if (!isJumping) {
        velocityY = SMALL_JUMP_VELOCITY;
        state = MarioState::Jump;
        if(is_grow) {
            this->SetImages(AnimationJumpGrow, 100, 0);
        }else {
            this->SetImages(AnimationJump, 100, 0);
        }
        std::shared_ptr<Util::SFX> jump_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/jump.mp3");
        jump_sfx->SetVolume(25);
        jump_sfx ->Play();
    }
}

void Mario::OnRun(const float distance) {

    if (distance == 0) return;

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

    X_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

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

bool Mario::CCDDCollides(glm::vec2 mario_pos, std::shared_ptr<BackgroundImage> box) {
    glm::vec2 a = mario_pos;
    glm::vec2 mario_size = this->m_Drawable->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    glm::vec2 b = box->m_Transform.translation;
    glm::vec2 b_size = box->GetSize();
    b_size.x *= box->GetScale().x;
    b_size.y *= box->GetScale().y;

    Y_state = CollisionState::None;
    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

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

bool Mario::GravityAndCollision(const float delta) {
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
            block->AfterCollisionEvents();
            // 固定在方塊下方開始下墜
            mario_y = block->GetTransform().translation.y - b_size.y / 2 - mario_size.y / 2;
            this->SetPosition({ mario_x, mario_y });
            break;
        }
    }
    if (Y_state == CollisionState::Top) {
        velocityY = 0;
        return true;
    }
    this->SetPosition({ mario_x, mario_y });

    // 如果沒有碰撞，表示在滯空狀態
    return !collision;
}

void Mario::UpdateAnimation() {
    const int direction = is_right_key_down - is_left_key_down;
    // facing left
    if (direction == -1) {
        m_Transform.scale = glm::vec2{-MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
    }
    // facing right
    if (direction == 1)  {
        m_Transform.scale = glm::vec2{MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
    }

    if (isJumping) {
        // 會影響到跳躍後的站立的動畫，註解掉
        // if(state != MarioState::Jump) {
        //     this->SetImages(AnimationJump);
        // }
        state = MarioState::Jump;
    } else {
        // 在地面上依據移動與否決定站立或跑步
        if (direction != 0) {
            if(state != MarioState::Run) {
                this->SetPlaying(true);
                this->SetLooping(true);
                if(is_grow) {
                    this->SetImages(AnimationRunGrow, 100, 0);
                }else {
                    this->SetImages(AnimationRun, 100, 0);
                }
            }
            isRunning = true;
            state = MarioState::Run;
        } else {
            if(state != MarioState::Stand) {
                this->SetPlaying(true);
                this->SetLooping(true);
                if (is_grow) {
                    this->SetImages(AnimationStandGrow, 100, 0);
                }else {
                    this->SetImages(AnimationStand, 100, 0);
                }
            }
            isRunning = false;
            state = MarioState::Stand;
        }
    }
}

float Mario::OnUpdate(const float delta) {
    // update moving
    const int direction = is_right_key_down - is_left_key_down;
    const float distance = direction * run_velocity * delta;
    OnRun(distance);

    isJumping = GravityAndCollision(3 * delta);

    // 每幀更新動畫圖片狀態
    UpdateAnimation();

    return distance;
}

float Mario::Move() {
    if (is_dead) {
        this->SetImages(this->AnimationDead, 100, 0);
        return 0.0f;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::DOWN)) {
        is_grow = not is_grow;
        // TODO fix animation not correct
        this->SetImages(this->AnimationGrow, 1000, 0);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
        is_left_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
        is_right_key_down = true;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::UP)) {
        OnJump();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        OnSmallJump();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::LEFT)) {
        is_left_key_down = false;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::RIGHT)) {
        is_right_key_down = false;
    }
    return OnUpdate(1);
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
    if (live == 0) SetImages(AnimationDead, 100, 0);
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