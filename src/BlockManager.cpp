//
// Created by 112590007 on 2025/3/14.
//

#include "BlockManager.hpp"
#include "Global.hpp"
#include "AnimatedImage.hpp"
#include "App.hpp"

#include <iostream>

BlockManager::BlockManager() {
    int imgidx_size = imgidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        if(imgidx[i] == 6 || imgidx[i] == 9) {
            m_Backgrounds.push_back(std::make_shared<AnimatedImage>());
            m_Backgrounds.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i] + 1],imagePaths[imgidx[i] + 2]}, 1000, 0);
        }else {
            m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
            m_Backgrounds.back()->SetImage(imagePaths[imgidx[i]]);
        }
        m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
    }

    // test block position

/**    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
    m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
    m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,3 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
    m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,4 * BLOCK_SIZE - 325.0f);**/
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
    default:
        return tmp_x;
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
    default:
        return tmp_y;
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
    default:
        return imgidx;
    }
}

void BlockManager::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds){
    this->m_Backgrounds.clear();
    this->m_Backgrounds = backgrounds;
}

std::vector<std::shared_ptr<BackgroundImage>> BlockManager::GetBackground(){
    return m_Backgrounds;
}
