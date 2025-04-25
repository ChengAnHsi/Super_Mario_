//
// Created by 112590007 on 2025/3/6.
//

#ifndef MARIO_HPP
#define MARIO_HPP
#include "AnimatedCharacter.hpp"
#include "BlockManager.hpp"
#include "PhaseResourceManger.hpp"
#include "CollisionState.hpp"

#include <string>

enum class MarioState {
    Stand,
    Run,
    Jump
};


class Mario : public AnimatedCharacter {
public:
    Mario(int coin, int live, int score, const std::vector<std::string>& AnimationPaths): AnimatedCharacter(AnimationPaths){
        this->coin = coin;
        this->live = live;
        this->score = score;
    };

    // move function
    void OnJump();
    void OnSmallJump();
    void OnKillJump();
    void OnRun(float distance);
    float Move();
    float OnUpdate(float delta);

    // collision function
    bool AABBCollides(glm::vec2 mario_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 mario_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta);

    void UpdateAnimation();

    // getter and setter
    void IncreaseCoin(int coin);
    [[nodiscard]] int GetCoin() const;
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void IncreaseScore(int score);
    [[nodiscard]] int GetScore() const;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes);
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks);
    void AddCollectibles(std::vector<std::shared_ptr<BackgroundImage>> collectibles);
    void ClearCollisionBoxes();
    void ClearCollectibles();
    void ClearCollisionBlocks();
    float velocityY = 0.0f;      // 角色在 Y 軸的速度
    bool is_dead = false;
    void Die(); // Handle Mario's death sequence
    void UpdateDeadState(float delta); // Update function for when Mario is dead
private:
    int coin = 0;
    int live = 3;
    int score = 0;
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    
    // Add these variables to track death state
    bool is_dying = false; // Transitional state between alive and dead
    float death_timer = 0.0f;
    const float DEATH_PAUSE_TIME = 60.0f; // 1 second at 60fps
    const float DEATH_JUMP_VELOCITY = 300.0f;
    bool collision_enabled = true;
    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;
    std::vector<std::shared_ptr<BackgroundImage>> collision_collectibles;

    MarioState state = MarioState::Stand;
    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isJumping = false;
    bool isRunning = false;
    float delta_time = 1.0f;
    float run_velocity = 5.0f;
    float jump_velocity = 12.7f;

    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    float JUMP_VELOCITY = 360.0f; // 跳躍初速度
    float SMALL_JUMP_VELOCITY = 250.0f; // 小跳躍初速度
    float MAX_JUMP_VELOCITY = 55.0f; // 最高跳躍值

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/mario0.png",RESOURCE_DIR"/Entities/mario1.png",RESOURCE_DIR"/Entities/mario2.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/marioDead.png"};
    std::vector<std::string> AnimationJump = {RESOURCE_DIR"/Entities/mario_Jump.png"};
    std::vector<std::string> AnimationStand = {RESOURCE_DIR"/Entities/mario_stand.png"};

    std::vector<std::string> AnimationGrow = {RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_stand.png",
        RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_stand.png", RESOURCE_DIR"/Entities/mario_grown_squat.png",
    RESOURCE_DIR"/Entities/mario_grown_stand.png",RESOURCE_DIR"/Entities/mario_grown_squat.png", RESOURCE_DIR"/Entities/mario_stand.png",
    RESOURCE_DIR"/Entities/mario_grown_squat.png",RESOURCE_DIR"/Entities/mario_grown_stand.png"};
    std::vector<std::string> AnimationRunGrow = {RESOURCE_DIR"/Entities/mario_grown0.png",RESOURCE_DIR"/Entities/mario_grown1.png",RESOURCE_DIR"/Entities/mario_grown2.png"};
    std::vector<std::string> AnimationJumpGrow = {RESOURCE_DIR"/Entities/mario_grown_Jump.png"};
    std::vector<std::string> AnimationStandGrow = {RESOURCE_DIR"/Entities/mario_grown_stand.png"};

    bool is_grow = false;

};
#endif //MARIO_HPP

