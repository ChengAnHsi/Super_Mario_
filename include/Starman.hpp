//
// Created by 112590007 on 2025/4/11.
//
#ifndef STARMANAN_HPP
#define STARMANAN_HPP

#include "PropsManager.hpp"

class Starman : public Props {
public:
    Starman() = default;

    void AfterCollisionEvents() override;
};

#endif //STARMANAN_HPP