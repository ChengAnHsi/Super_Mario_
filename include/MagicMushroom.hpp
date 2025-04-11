//
// Created by 112590007 on 2025/4/11.
//
#ifndef MAGICMUSHROOM_HPP
#define MAGICMUSHROOM_HPP

#include "PropsManager.hpp"

class MagicMushroom : public Props {
public:
    MagicMushroom() = default;

    void AfterCollisionEvents() override;
};

#endif //MAGICMUSHROOM_HPP