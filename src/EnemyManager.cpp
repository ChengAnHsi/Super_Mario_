//
// Created by 112590007 on 2025/3/14.
//
#include "EnemyManager.hpp"
#include "Global.hpp"
#include "App.hpp"
#include <iostream>

EnemyManager::EnemyManager() {
    int imgidx_size = imgidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE - 335.0f);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->SetImage(imageFiles[imgidx[i]]);
        m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
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
