//
// Created by 112590007 on 2025/3/14.
//
#include "BlockManager.hpp"
#include "Global.hpp"
#include "App.hpp"
#include <iostream>

BlockManager::BlockManager() {
    int imgidx_size = imgidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        // position should change to correct position
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE - 335.0f);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(imageFiles[imgidx[i]]);
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
    }

    for (int i = 0; i < 224; i++) {
        if ( i == 69 || i == 70 || (i <= 88 && i >= 86) || i == 153 || i == 154){
            continue;
        }
        // correct x position or cut small floor to solve align and hole
        // x: i * BLOCK_SIZE + (22.0f * i / 8)
        // or: i * BLOCK_SIZE - 380.0f
        // first floor
        m_PositionX.push_back(i * BLOCK_SIZE - 335.0f);
        m_PositionY.push_back(BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(imageFiles[0]);
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX.back(),m_PositionY.back());

        // second floor
        m_PositionX.push_back(i * BLOCK_SIZE - 335.0f);
        m_PositionY.push_back( - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(imageFiles[0]);//RESOURCE_DIR"/Scenery/Overworld/floorbricks.png"
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX.back(),m_PositionY.back());
    }

    // test block position

    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_PositionX.push_back(6 * BLOCK_SIZE - 335.0f);
    m_PositionY.push_back( 2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.back()->SetPosition(6 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_PositionX.push_back(6 * BLOCK_SIZE - 335.0f);
    m_PositionY.push_back( 2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.back()->SetPosition(8 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(9 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
    /**m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(4 * BLOCK_SIZE - 335.0f + BLOCK_SIZE/2,2 * BLOCK_SIZE - 325.0f - BLOCK_SIZE/2);**/
}

std::vector<float> BlockManager::GetPosX() {
    return m_PositionX;
}

std::vector<int> BlockManager::GetX(int phase){
    std::cout << phase;
    switch (phase)
    {
    case 1:
        return tmp_x;
    case 2:
        return tmp_x2;
    case 3:
        return tmp_x3;
    }
}

std::vector<int> BlockManager::GetY(int phase){
    switch (phase)
    {
    case 1:
        return tmp_y;
    case 2:
        return tmp_y2;
    case 3:
        return tmp_y3;
    }
}

std::vector<int> BlockManager::Getidx(int phase){
    switch (phase)
    {
    case 1:
        return imgidx;
    case 2:
        return imgidx2;
    case 3:
        return imgidx3;
    }
}

void BlockManager::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds){
    /**for (int i = 0; i < m_Backgrounds.size(); i++){
        m_Backgrounds[i]->SetPosition(0, 0);
        m_Backgrounds[i]->SetVisible(false);
    }**/
    this->m_Backgrounds = backgrounds;
}

std::vector<std::shared_ptr<BackgroundImage>> BlockManager::GetBackground(){
    return m_Backgrounds;
}
