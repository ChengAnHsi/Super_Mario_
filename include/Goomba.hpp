//
// Created by 112590007 on 2025/3/6.
//

#ifndef GOOMBA_HPP
#define GOOMBA_HPP
#include "AnimatedCharacter.hpp"
#include "BlockManager.hpp"
#include "PhaseResourceManger.hpp"
#include "Enemy.hpp"
#include <string>

enum class GoombaState {
    Stand,
    Run,
};


class Goomba : public Enemy{
public:
    Goomba(int live, const std::vector<std::string>& AnimationPaths){
        this->live = live;
    };

    // move function
    void move() override;
    float OnUpdate(float delta);
    void OnRun(float distance);

    // collision function
    bool GravityAndCollision(float delta) override;

    void UpdateAnimation(const float delta);

    // getter and setter
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    void AddCollisionBox(std::vector<std::shared_ptr<BackgroundImage>> box);
    void ClearCollisionBox();

private:
    int live = 1;
    int score = 0;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<BackgroundImage>> collision_collectibles;

    MarioState state = MarioState::Stand;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;


    bool isRunning = false;
    bool isfacingright = false;
    float delta_time = 1.0f;
    float run_velocity = 5.0f;
    float velocityY = 0.0f;      // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    float JUMP_VELOCITY = 360.0f; // 跳躍初速度
    float SMALL_JUMP_VELOCITY = 250.0f; // 小跳躍初速度
    float MAX_JUMP_VELOCITY = 55.0f; // 最高跳躍值

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Overworld/goomba0.png",RESOURCE_DIR"/Entities/Overworld/goomba1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/Overworld/goombaDead.png"};

    bool is_dead = false;
};
#endif //GOOMBA_HPP

