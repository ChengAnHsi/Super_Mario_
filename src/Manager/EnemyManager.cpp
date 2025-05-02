//
// Created by 112590007 on 2025/3/14.
//

#include <iostream>

#include "Global.hpp"
#include "App.hpp"

#include "Enemies/Flower.hpp"
#include "Enemies/FlyKoopa.hpp"
#include "Enemies/Koopa.hpp"
#include "Enemies/Goomba.hpp"

#include "Manager/EnemyManager.hpp"

EnemyManager::EnemyManager() {
    size_t imgidx_size = imgidx.size();
    for (size_t i = 0; i < imgidx_size; i++) {
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);

        if(imgidx[i] == 2 || imgidx[i] == 5){
            m_Enemies.push_back(std::make_shared<Goomba>());
        }
        else if(imgidx[i] == 0 || imgidx[i] == 8){
            m_Enemies.push_back(std::make_shared<Flower>());
        }
        else if(imgidx[i] == 10 || imgidx[i] == 16 || imgidx[i] == 20){
            m_Enemies.push_back(std::make_shared<Koopa>());
        }
        else if(imgidx[i] == 18){
            m_Enemies.push_back(std::make_shared<FlyKoopa>());
            if (std::shared_ptr<FlyKoopa> flykoopa = std::dynamic_pointer_cast<FlyKoopa>(m_Enemies.back())) {
                float flight_height = 3.0f;

                if (m_PositionX[i] < 80) {
                    flight_height = 2.0f;
                } else if (m_PositionX[i] > 100) {
                    flight_height = 100.0f;
                }

                flykoopa->SetFlightHeight(flight_height);
            }
        }

        // First set the image so the dimensions are available
        m_Enemies.back()->SetImage({imageFiles[imgidx[i]]}, 1000, 0);
        m_Enemies.back()->SetScale(ENEMY_MAGNIFICATION, ENEMY_MAGNIFICATION);

        // Then set the position (which will trigger UpdateYMovementRange for Flower instances)
        m_Enemies.back()->SetPosition(m_PositionX[i], m_PositionY[i]);

        // For Flower type, ensure the Y range is calculated
        if (std::shared_ptr<Flower> flower = std::dynamic_pointer_cast<Flower>(m_Enemies.back())) {
            // For flowers, we explicitly update the Y movement range after setting position and image
            flower->UpdateYMovementRange();
        }
    }
}

std::vector<float> EnemyManager::GetX(int phase){
    std::cout << phase;
    switch (phase)
    {
    case 1:
        return tmp_x;
    case 2:
        return tmp_x2;
    case 3:
        return tmp_x3;
    default:
        return tmp_x;
    }
}

std::vector<float> EnemyManager::GetY(int phase){
    switch (phase)
    {
    case 1:
        return tmp_y;
    case 2:
        return tmp_y2;
    case 3:
        return tmp_y3;
    default:
        return tmp_y;
    }
}

std::vector<int> EnemyManager::Getidx(int phase){
    switch (phase)
    {
    case 1:
        return imgidx;
    case 2:
        return imgidx2;
    case 3:
        return imgidx3;
    default:
        return imgidx;
    }
}

void EnemyManager::SetEnemyMoving(){
    for (const auto& enemy : m_Enemies){
        // map size divide by 2 = 360
        if (enemy->GetPosition().x < 360){
            enemy->SetMoving(true);
        }
        enemy->Move();
    }
}

void EnemyManager::SetEnemies(std::vector<std::shared_ptr<Enemy>> enemies){
    m_Enemies.clear();
    m_Enemies = enemies;

    // For any flower instances in the new enemy list, update their Y movement range
    for (const auto& enemy : m_Enemies) {
        if (std::shared_ptr<Flower> flower = std::dynamic_pointer_cast<Flower>(enemy)) {
            flower->UpdateYMovementRange();
        }
    }
}

std::vector<std::shared_ptr<Enemy>> EnemyManager::GetEnemies(){
    return m_Enemies;
}

void EnemyManager::SetAllEnemyCollisionBoxs(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& enemy : m_Enemies) {
        enemy->AddCollisionBoxes(boxes);
    }
}

void EnemyManager::SetAllEnemyCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks){
    for (const auto& enemy : m_Enemies) {
        enemy->AddCollisionBlocks(blocks);
    }
}
void EnemyManager::CheckMarioCollisions(std::shared_ptr<Mario> mario) {
    for (const auto& enemy : m_Enemies) {
        // Dynamic cast to check if the enemy is a Goomba
        if (auto goomba = std::dynamic_pointer_cast<Goomba>(enemy)) {
            goomba->CheckMarioCollision(mario);
        }
        if (auto koopa = std::dynamic_pointer_cast<Koopa>(enemy)) {
            koopa->CheckMarioCollision(mario);
        }
    }
}