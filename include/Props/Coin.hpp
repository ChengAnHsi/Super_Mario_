//
// Created by 112590007 on 2025/4/25.
//

#ifndef COIN_HPP
#define COIN_HPP

#include "Manager/PropsManager.hpp"
#include "Global.hpp"

class Coin : public Props {
public:
    Coin() = default;
    ~Coin() override = default;

    void SpawnProps() override;
    void Update(float dt) override;

private:
    bool is_spawning = false;
    bool is_goingup = false;
    PropsState state = PropsState::Not_Activated;
    float remaining_distance = BLOCK_SIZE;
    float velocityY = 0.0f;
};

#endif //COIN_HPP