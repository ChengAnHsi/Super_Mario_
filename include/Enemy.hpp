//
// Created by 112590007 on 2025/4/4.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <vector>
#include <string>

#include "Mario.hpp"
#include "Animatedimage.hpp"
#include "BackgroundImage.hpp"
#include "Util/Animation.hpp"


class Enemy : public AnimatedImage {

public:
    Enemy() = default;

    Enemy(const std::vector<std::string>& AnimationPaths, int interval, int cooldown) {
        // use different instructor to setting animation
        SetImage(AnimationPaths, interval, cooldown);
    }
    virtual void move() = 0;
    bool isMoving;
    void SetMoving(bool moving) { isMoving = moving; }
    bool AABBCollides(glm::vec2 enemy_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 enemy_pos, std::shared_ptr<BackgroundImage> box);
    virtual bool GravityAndCollision(float delta) = 0;
    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
};
#endif //ENEMY_HPP
