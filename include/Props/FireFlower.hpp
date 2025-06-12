//
// Created by 112590007 on 2025/4/11.
//

#ifndef FIREFLOWER_HPP
#define FIREFLOWER_HPP

#include "Manager/PropsManager.hpp"
#include "Global.hpp"

class FireFlower final : public Props {
public:
    FireFlower() = default;
    ~FireFlower() override = default;

    void SpawnProps() override;
    void Update(float dt) override;
    void AfterCollisionEvents(std::shared_ptr<Mario> mario) override;

    // move function
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;

private:
    bool is_spawning = false;
    float remaining_distance = BLOCK_SIZE;
    float velocityY = 0.0f;
    float GRAVITY = -300.0f;
};

#endif //FIREFLOWER_HPP