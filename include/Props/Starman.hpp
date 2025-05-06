//
// Created by 112590007 on 2025/4/11.
//

#ifndef STARMANAN_HPP
#define STARMANAN_HPP

#include "Manager/PropsManager.hpp"
#include "Global.hpp"

class Starman : public Props {
public:
    Starman() = default;

    void SpawnProps() override;
    void Update(float dt) override;
    void AfterCollisionEvents(std::shared_ptr<Mario> mario) override;

    void UpdateAnimation() override;

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;
    void Jump();

    // collision function
    bool AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;

private:
    bool is_spawning = false;
    float remaining_distance = BLOCK_SIZE;
    float velocityY = 0.0f; // 角色在 Y 軸的速度
    float GRAVITY = -300.0f; // 重力值，現在是以 px/s² 為單位
    float velocityX = 4.0f;  // 左右移動用
    bool isJumping = false;
    float JUMP_VELOCITY = 250.0f; // 跳躍初速度
};

#endif //STARMANAN_HPP