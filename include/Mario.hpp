//
// Created by 112590007 on 2025/3/6.
//

#ifndef MARIO_HPP
#define MARIO_HPP

#include <string>

#include "AnimatedCharacter.hpp"
#include "CollisionState.hpp"
#include "Blocks/FlyPlatfrom.hpp"

#include "Manager/FireballManager.hpp"

enum class MarioState {
    Stand,
    Run,
    Jump,
    Squat
};

enum class DrillState {
    Left,
    Right,
    Up,
    Down,
    None
};

class Mario final : public AnimatedCharacter {
public:
    explicit Mario(const std::vector<std::string>& AnimationPaths): AnimatedCharacter(AnimationPaths){};
    ~Mario() override = default;

    // move function
    void OnJump();
    void OnSmallJump();
    void OnKillJump();
    void OnRun(float distance);
    float Move();
    float OnUpdate(float delta);
    void Fire();
    void Die(); // Handle Mario's death sequence

    // collision function
    bool CollidesAndSetDirection(glm::vec2 mario_pos, const std::shared_ptr<BackgroundImage>& box, bool is_checkX);
    bool GravityAndCollision(float delta);

    // animation
    void UpdateAnimation();
    void SetGrowingAnimation();
    void SetJumpAnimation();
    void UpdateGrowingState();
    void UpdateDeadState(float delta); // Update function for when Mario is dead
    void PullFlag();
    void DrillTube();

    // getter and setter
    void IncreaseCoin(int coin);
    [[nodiscard]] int GetCoin() const;
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void IncreaseScore(int score);
    [[nodiscard]] int GetScore() const;
    [[nodiscard]] float GetVelocityY() const;
    void SetFireballManager(const std::shared_ptr<FireballManager> &FM);

    // state
    void SetGrow(bool is_grow);
    [[nodiscard]] bool GetGrow() const;
    [[nodiscard]] bool GetGrowing() const;
    void SetFire(bool is_fire);
    void SetInvincible(bool is_invincible);
    [[nodiscard]] bool GetInvincible() const;
    [[nodiscard]] bool GetTempInvincible() const;
    [[nodiscard]] bool GetFire() const;
    void SetDead(bool is_dead);
    [[nodiscard]] bool GetDead() const;
    void SetDying(bool is_dying);
    [[nodiscard]] bool GetDying() const;

    void SetPull(bool is_pull);
    [[nodiscard]] bool GetPull() const;
    void SetDrill(bool is_drill);
    [[nodiscard]] bool GetDrill() const;
    void SetDrillState(DrillState drill_state);
    void SetDrillDistance(float drill_tube_dis);
    [[nodiscard]] bool GetBackToCastle() const;
    [[nodiscard]] bool GetReadyNextPhase() const;
    void SetTimeToMoveCamera(bool is_time_to_move_camera_map2);
    [[nodiscard]] bool GetTimeToMoveCamera() const;

    void ResetStateForNextPhase();

    void AddCollisionBoxes(const std::vector<std::shared_ptr<BackgroundImage>>& boxes);
    void AddCollisionBlocks(const std::vector<std::shared_ptr<Block>>& blocks);
    void AddCollisionFlyPlatForms(const std::vector<std::shared_ptr<FlyPlatform>>& platforms);
    void AddCollectibles(const std::vector<std::shared_ptr<BackgroundImage>>& collectibles);
    void ClearCollisionBoxes();
    void ClearCollectibles();
    void ClearFlyPlatForms();
    void ClearCollisionBlocks();

private:
    int coin = 0;
    int live = 3;
    int score = 0;
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_down_key_down = false;
    bool is_run_key_down = false;

    // growing timer
    float growing_timer = 0.0f;
    float GROWING_TIME = 60.0f;

    // track death state
    float death_timer = 0.0f; // Time to start jumping
    const float DEATH_PAUSE_TIME = 60.0f; // 1 second at 60fps
    const float DEATH_JUMP_VELOCITY = 300.0f;

    // track invincible state
    float invincible_timer = 0.0f; // The countdown ends when the invincibility time (3 sec) ends
    const float TEMP_INVINCIBLE_END_TIME = 180.0f;
    const float INVINCIBLE_END_TIME = 1800.0f;

    // fireball shoot timer
    float shoot_fireball_timer = 30.0f;
    const float FIREBALL_SHOOT_TIME = 30.0f;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;
    std::vector<std::shared_ptr<FlyPlatform>> collision_flyplatforms;
    std::vector<std::shared_ptr<BackgroundImage>> collision_collectibles;
    std::shared_ptr<FireballManager> m_FM;

    MarioState mario_state = MarioState::Stand;
    DrillState drill_state = DrillState::None;
    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isJumping = false;
    bool isRunning = false;
    bool isFacingRight = true;

    bool is_grow = false;
    bool is_growing = false;
    bool is_fire = false;
    bool is_invincible = false;
    bool is_temporarily_invincible = false;
    bool is_dead = false; // game over or not
    bool is_dying = false; // Transitional state between alive and dead

    bool is_pull = false;
    bool is_drill = false;
    bool is_back_to_castle = false;
    bool is_time_to_move_camera_map2 = false;
    bool is_ready_for_next_phase = false;

    float delta_time = 1.0f;
    float back_to_castle_dis = 0.0f;
    float drill_tube_dis = 0.0f;

    float velocityX = 5.0f;
    float velocityY = 0.0f;
    float GRAVITY = -300.0f; // 重力值，以 px/s² 為單位
    float JUMP_VELOCITY = 360.0f;
    float SMALL_JUMP_VELOCITY = 250.0f;

    std::vector<std::string> AnimationFireLast = {}; // Restore the previous animation of the throwing animation

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/mario0.png",RESOURCE_DIR"/Entities/mario1.png",RESOURCE_DIR"/Entities/mario2.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/marioDead.png"};
    std::vector<std::string> AnimationJump = {RESOURCE_DIR"/Entities/mario_Jump.png"};
    std::vector<std::string> AnimationStand = {RESOURCE_DIR"/Entities/mario_stand.png"};
    std::vector<std::string> AnimationPull = {RESOURCE_DIR"/Entities/mario_pull0.png", RESOURCE_DIR"/Entities/mario_pull1.png"};

    // m s m s m l s m l
    std::vector<std::string> AnimationGrow = {RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
         RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",RESOURCE_DIR"/Entities/mario_grown_stand_small.png", RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",
     RESOURCE_DIR"/Entities/mario_grown_stand.png", RESOURCE_DIR"/Entities/mario_grown_stand_small.png", RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",
     RESOURCE_DIR"/Entities/mario_grown_stand.png"};
    // todo: mario_grown_transparent?
    // s l s l s l s l s
    std::vector<std::string> AnimationShrink = {RESOURCE_DIR"/Entities/mario_grown_stand_small.png",RESOURCE_DIR"/Entities/mario_grown_stand.png",
             RESOURCE_DIR"/Entities/mario_grown_stand_small.png",RESOURCE_DIR"/Entities/mario_grown_stand.png", RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
         RESOURCE_DIR"/Entities/mario_grown_stand.png", RESOURCE_DIR"/Entities/mario_grown_stand_small.png", RESOURCE_DIR"/Entities/mario_grown_stand.png",
         RESOURCE_DIR"/Entities/mario_stand.png"};
    std::vector<std::string> AnimationRunGrow = {RESOURCE_DIR"/Entities/mario_grown0.png",RESOURCE_DIR"/Entities/mario_grown1.png",RESOURCE_DIR"/Entities/mario_grown2.png"};
    std::vector<std::string> AnimationJumpGrow = {RESOURCE_DIR"/Entities/mario_grown_Jump.png"};
    std::vector<std::string> AnimationStandGrow = {RESOURCE_DIR"/Entities/mario_grown_stand.png"};
    std::vector<std::string> AnimationSquatGrow = {RESOURCE_DIR"/Entities/mario_grown_squat.png"};
    std::vector<std::string> AnimationPullGrow = {RESOURCE_DIR"/Entities/mario_grown_pull0.png", RESOURCE_DIR"/Entities/mario_grown_pull1.png"};

    std::vector<std::string> AnimationRunGrowFire = {RESOURCE_DIR"/Entities/mario_fire0.png",RESOURCE_DIR"/Entities/mario_fire1.png",RESOURCE_DIR"/Entities/mario_fire2.png"};
    std::vector<std::string> AnimationJumpGrowFire = {RESOURCE_DIR"/Entities/mario_fire_Jump.png"};
    std::vector<std::string> AnimationStandGrowFire = {RESOURCE_DIR"/Entities/mario_fire_stand.png"};
    std::vector<std::string> AnimationSquatGrowFire = {RESOURCE_DIR"/Entities/mario_fire_squat.png"};

    std::vector<std::string> AnimationRunInvincible = {RESOURCE_DIR"/Entities/mario0_Invinci0.png", RESOURCE_DIR"/Entities/mario0_Invinci1.png", RESOURCE_DIR"/Entities/mario0_Invinci2.png", RESOURCE_DIR"/Entities/mario0.png",
        RESOURCE_DIR"/Entities/mario1_Invinci0.png", RESOURCE_DIR"/Entities/mario1_Invinci1.png", RESOURCE_DIR"/Entities/mario1_Invinci2.png",RESOURCE_DIR"/Entities/mario1.png",
        RESOURCE_DIR"/Entities/mario2_Invinci0.png", RESOURCE_DIR"/Entities/mario2_Invinci1.png", RESOURCE_DIR"/Entities/mario2_Invinci2.png",RESOURCE_DIR"/Entities/mario2.png"};
    std::vector<std::string> AnimationJumpInvincible = {RESOURCE_DIR"/Entities/mario_Jump_Invinci0.png", RESOURCE_DIR"/Entities/mario_Jump_Invinci1.png", RESOURCE_DIR"/Entities/mario_Jump_Invinci2.png",RESOURCE_DIR"/Entities/mario_Jump.png"};
    std::vector<std::string> AnimationStandInvincible = {RESOURCE_DIR"/Entities/mario_stand_Invinci0.png", RESOURCE_DIR"/Entities/mario_stand_Invinci1.png", RESOURCE_DIR"/Entities/mario_stand_Invinci2.png",RESOURCE_DIR"/Entities/mario_stand.png"};

    std::vector<std::string> AnimationGrowInvincible = {RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci0.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
             RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci1.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci2.png",
         RESOURCE_DIR"/Entities/mario_grown_stand.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",
         RESOURCE_DIR"/Entities/mario_grown_stand.png"};

    // todo: mario_grown_stand_small + fire & mario_grown_stand_mid + fire state
    std::vector<std::string> AnimationGrowFireInvincible = {RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci0.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
             RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci1.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_mid_invinci2.png",
         RESOURCE_DIR"/Entities/mario_fire_stand.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_small.png",
        RESOURCE_DIR"/Entities/mario_grown_stand_mid.png",
         RESOURCE_DIR"/Entities/mario_fire_stand.png"};

    std::vector<std::string> AnimationRunGrowInvincible = {RESOURCE_DIR"/Entities/mario_grown0_invinci0.png",RESOURCE_DIR"/Entities/mario_grown0_invinci1.png",RESOURCE_DIR"/Entities/mario_grown0_invinci2.png",RESOURCE_DIR"/Entities/mario_grown0.png",
        RESOURCE_DIR"/Entities/mario_grown1_invinci0.png",RESOURCE_DIR"/Entities/mario_grown1_invinci1.png",RESOURCE_DIR"/Entities/mario_grown1_invinci2.png",RESOURCE_DIR"/Entities/mario_grown1.png",
        RESOURCE_DIR"/Entities/mario_grown2_invinci0.png",RESOURCE_DIR"/Entities/mario_grown2_invinci1.png",RESOURCE_DIR"/Entities/mario_grown2_invinci2.png",RESOURCE_DIR"/Entities/mario_grown2.png"};
    std::vector<std::string> AnimationJumpGrowInvincible = {RESOURCE_DIR"/Entities/mario_grown_Jump_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_Jump_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_Jump_invinci2.png",RESOURCE_DIR"/Entities/mario_grown_Jump.png"};
    std::vector<std::string> AnimationStandGrowInvincible = {RESOURCE_DIR"/Entities/mario_grown_stand_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_stand_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_stand_invinci2.png",RESOURCE_DIR"/Entities/mario_grown_stand.png"};
    std::vector<std::string> AnimationSquatGrowInvincible = {RESOURCE_DIR"/Entities/mario_grown_squat_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_squat_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_squat_invinci2.png",RESOURCE_DIR"/Entities/mario_grown_squat.png"};

    std::vector<std::string> AnimationRunGrowFireInvincible = {RESOURCE_DIR"/Entities/mario_grown0_invinci0.png",RESOURCE_DIR"/Entities/mario_grown0_invinci1.png",RESOURCE_DIR"/Entities/mario_grown0_invinci2.png",RESOURCE_DIR"/Entities/mario_fire0.png",
        RESOURCE_DIR"/Entities/mario_grown1_invinci0.png",RESOURCE_DIR"/Entities/mario_grown1_invinci1.png",RESOURCE_DIR"/Entities/mario_grown1_invinci2.png",RESOURCE_DIR"/Entities/mario_fire1.png",
        RESOURCE_DIR"/Entities/mario_grown2_invinci0.png",RESOURCE_DIR"/Entities/mario_grown2_invinci1.png",RESOURCE_DIR"/Entities/mario_grown2_invinci2.png",RESOURCE_DIR"/Entities/mario_fire2.png"};
    std::vector<std::string> AnimationJumpGrowFireInvincible = {RESOURCE_DIR"/Entities/mario_grown_Jump_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_Jump_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_Jump_invinci2.png",RESOURCE_DIR"/Entities/mario_fire_Jump.png"};
    std::vector<std::string> AnimationStandGrowFireInvincible = {RESOURCE_DIR"/Entities/mario_grown_stand_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_stand_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_stand_invinci2.png",RESOURCE_DIR"/Entities/mario_fire_stand.png"};
    std::vector<std::string> AnimationSquatGrowFireInvincible = {RESOURCE_DIR"/Entities/mario_grown_squat_invinci0.png",RESOURCE_DIR"/Entities/mario_grown_squat_invinci1.png",RESOURCE_DIR"/Entities/mario_grown_squat_invinci2.png",RESOURCE_DIR"/Entities/mario_fire_squat.png"};
};
#endif //MARIO_HPP

