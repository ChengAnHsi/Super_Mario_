//
// Created by 112590007 on 2025/3/14.
//

#include "BlockManager.hpp"
#include "Block.hpp"
#include "MysteryBlock.hpp"
#include "CommonBlock.hpp"
#include "ImmovableBlock.hpp"
#include "BackgroundImage.hpp"
#include "Global.hpp"
#include "App.hpp"
#include <iostream>

#include "OneUpMushroom.hpp"
#include "../PTSD/lib/sdl2/src/joystick/windows/SDL_windowsjoystick_c.h"

BlockManager::BlockManager() {
    int imgidx_size = imgidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        if(imgidx[i] == 6 || imgidx[i] == 9) {
            auto temp = std::make_shared<MysteryBlock>();
            temp->SetProps(std::make_shared<OneUpMushroom>());
            m_Blocks.push_back(temp);
            m_Blocks.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i] + 1],imagePaths[imgidx[i] + 2]}, 1000, 0);
        }else if(imgidx[i] == 0 || imgidx[i] == 1) {
            m_Blocks.push_back(std::make_shared<CommonBlock>());
            m_Blocks.back()->SetImage(imagePaths[imgidx[i]]);
        }else {
            m_Blocks.push_back(std::make_shared<ImmovableBlock>());
            m_Blocks.back()->SetImage(imagePaths[imgidx[i]]);
        }
        m_Blocks.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Blocks.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
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

void BlockManager::SetBlocks(std::vector<std::shared_ptr<Block>> blocks){
    this->m_Blocks.clear();
    this->m_Blocks = blocks;
}

std::vector<std::shared_ptr<BackgroundImage>> BlockManager::GetBackground() {
    std::vector<std::shared_ptr<BackgroundImage>> all_block;
    for (int i = 0; i < m_Blocks.size(); i++) {
        all_block.push_back(std::static_pointer_cast<BackgroundImage>(m_Blocks[i]));
    }
    return all_block;
}

std::vector<std::shared_ptr<Block>> BlockManager::GetBlocks(){
    return m_Blocks;
}
// #include "BlockManager.hpp"
// #include "Global.hpp"
// #include "AnimatedImage.hpp"
// #include "App.hpp"
//
// #include <iostream>
//
// BlockManager::BlockManager() {
//     int imgidx_size = imgidx.size();
//     for (int i = 0; i < imgidx_size; i++) {
//         m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
//         m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
//         if(imgidx[i] == 6 || imgidx[i] == 9) {
//             m_Backgrounds.push_back(std::make_shared<AnimatedImage>());
//             m_Backgrounds.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i] + 1],imagePaths[imgidx[i] + 2]}, 1000, 0);
//         }else {
//             m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
//             m_Backgrounds.back()->SetImage(imagePaths[imgidx[i]]);
//         }
//         m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
//         m_Backgrounds.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
//     }
//
//     // test block position
//
// /**    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
//     m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
//     m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
//     m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,2 * BLOCK_SIZE - 325.0f);
//     m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
//     m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
//     m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
//     m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,3 * BLOCK_SIZE - 325.0f);
//     m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
//     m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png");
//     m_Backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
//     m_Backgrounds.back()->SetPosition(5 * BLOCK_SIZE - 335.0f,4 * BLOCK_SIZE - 325.0f);**/
// }
//
// std::vector<float> BlockManager::GetPosX() {
//     return m_PositionX;
// }
//
// std::vector<int> BlockManager::GetX(int phase){
//     std::cout << phase;
//     switch (phase)
//     {
//     case 1:
//         return tmp_x;
//     case 2:
//         return tmp_x2;
//     case 3:
//         return tmp_x3;
//     default:
//         return tmp_x;
//     }
// }
//
// std::vector<int> BlockManager::GetY(int phase){
//     switch (phase)
//     {
//     case 1:
//         return tmp_y;
//     case 2:
//         return tmp_y2;
//     case 3:
//         return tmp_y3;
//     default:
//         return tmp_y;
//     }
// }
//
// std::vector<int> BlockManager::Getidx(int phase){
//     switch (phase)
//     {
//     case 1:
//         return imgidx;
//     case 2:
//         return imgidx2;
//     case 3:
//         return imgidx3;
//     default:
//         return imgidx;
//     }
// }
//
// void BlockManager::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds){
//     this->m_Backgrounds.clear();
//     this->m_Backgrounds = backgrounds;
// }
//
// std::vector<std::shared_ptr<BackgroundImage>> BlockManager::GetBackground(){
//     return m_Backgrounds;
// }
