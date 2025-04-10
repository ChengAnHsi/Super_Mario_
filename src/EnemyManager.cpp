//
// Created by 112590007 on 2025/3/14.
//
#include "EnemyManager.hpp"
#include "Global.hpp"
#include "App.hpp"
#include <iostream>
#include "Goomba.hpp"
EnemyManager::EnemyManager() {
    int count = tmp_x.size();
    for (int i = 0; i < count; i++) {
        float posX = tmp_x[i] * BLOCK_SIZE - 335.0f;
        float posY = tmp_y[i] * BLOCK_SIZE - 325.0f;

        std::vector<std::string> goombaAnimations = {
            imageFiles[imgidx[i]]
        };
        auto enemy = std::make_shared<Goomba>(1, goombaAnimations);

        enemy->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        enemy->SetPosition(posX, posY);

        enemy->SetMoving(false);
        m_Enemies.push_back(enemy);
    }
}
void EnemyManager::SetEnemyMoving() {
    // 如果沒有敵人，則建立一個新的 Goomba 並啟動移動
    if (m_Enemies.empty()) {
        std::vector<std::string> goombaAnimations = {
            imageFiles[2]  // 例如使用 goomba0.png
        };
        auto enemy = std::make_shared<Goomba>(1, goombaAnimations);
        enemy->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        enemy->SetPosition(0, 0);  // 預設位置，可依需求調整
        enemy->SetMoving(true);
        m_Enemies.push_back(enemy);
    } else {
        // 遍歷所有現有敵人，更新 isMoving 狀態
        for (auto &enemy : m_Enemies) {
            if (enemy->GetPosition().x < 360.0f) {
                enemy->SetMoving(true);
            }
        }
    }
}
void EnemyManager::move(float delta) {
    for (auto &enemy : m_Enemies) {
        enemy->move();
    }
}
std::vector<int> EnemyManager::GetPosX() {
    return m_PositionX;
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

void EnemyManager::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds){
    this->m_Backgrounds = backgrounds;
}

std::vector<std::shared_ptr<BackgroundImage>> EnemyManager::GetBackground(){
    return m_Backgrounds;
}
