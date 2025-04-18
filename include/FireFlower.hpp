//
// Created by 112590007 on 2025/4/11.
//
#ifndef FIREFLOWER_HPP
#define FIREFLOWER_HPP

#include "PropsManager.hpp"

class FireFlower : public Props {
public:
    FireFlower() = default;

    void AfterCollisionEvents() override;
    void SpawnProps() override;
};

#endif //FIREFLOWER_HPP