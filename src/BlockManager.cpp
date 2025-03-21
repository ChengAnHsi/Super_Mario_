//
// Created by 112590007 on 2025/3/14.
//
#include "BlockManager.hpp"
#include "Global.hpp"
#include <iostream>

BlockManger::BlockManger() {
    int imgidx_size = imgidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        // position should change to correct position
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE - 380.0f);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(imageFiles[imgidx[i]]);
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
    }

    for (int i = 0; i < 224; i++) {
<<<<<<< HEAD
        if ( i == 69 || i == 70 || (i <= 88 && i >= 86) || i == 153 || i == 154){
            continue;
        }
        // correct x position or cut small floor to solve align and hole
        // x: i * BLOCK_SIZE + (22.0f * i / 8)
        // or: i * BLOCK_SIZE - 380.0f
=======
        if (i == 69 || i == 70 || (i <= 88 && i >= 86) || i == 153 || i == 154) {
            continue;
        }
        // first floor
>>>>>>> 4fb2e60b855d2a979359575c0f6edb317a56864d
        m_PositionX.push_back(i * BLOCK_SIZE - 380.0f);
        m_PositionY.push_back(BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Scenery/Overworld/floorbricks.png");
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX.back(),m_PositionY.back());

        // second floor
        m_PositionX.push_back(i * BLOCK_SIZE - 380.0f);
        m_PositionY.push_back( - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Scenery/Overworld/floorbricks.png");
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX.back(),m_PositionY.back());
    }

    // test map position(floor and block distance)

    /**m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(16 * BLOCK_SIZE - 380.0f,4 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(16 * BLOCK_SIZE - 380.0f,3 * BLOCK_SIZE - 325.0f);**/

    /**m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(4 * BLOCK_SIZE - 335.0f + BLOCK_SIZE/2,2 * BLOCK_SIZE - 325.0f - BLOCK_SIZE/2);**/
}

std::vector<int> BlockManger::GetX(int phase){
    std::cout << phase;
    switch (phase)
    {
    case 1:
        return tmp_x;
    case 2:
        return tmp_x2;
    default:
        return tmp_x;
    }
}

std::vector<int> BlockManger::GetY(int phase){
    switch (phase)
    {
    case 1:
        return tmp_y;
    case 2:
        return tmp_y2;
    default:
        return tmp_y;
    }
}

std::vector<int> BlockManger::Getidx(int phase){
    switch (phase)
    {
    case 1:
        return imgidx;
    case 2:
        return imgidx2;
    default:
        return imgidx;
    }
}

void BlockManger::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds){
    /**for (int i = 0; i < m_Backgrounds.size(); i++){
        m_Backgrounds[i]->SetPosition(0, 0);
        m_Backgrounds[i]->SetVisible(false);
    }**/
    this->m_Backgrounds = backgrounds;//fck
}

void BlockManger::SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds, Util::Renderer m_Root){
    std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
    for (const auto & img : m_Backgrounds) {
        all_obj.emplace_back(img);
        m_Root.RemoveChild(all_obj.back());
    }
    /**for (int i = 0; i < m_Backgrounds.size(); i++){
        m_Backgrounds[i]->SetPosition(0, 0);
        m_Backgrounds[i]->SetVisible(false);
    }**/
    this->m_Backgrounds = backgrounds;
}
/**int BlockManger::block_visible(float camera_movement_dis, int idx) {
   for (int i = idx; i < imgidx.size(); i++) {
        std::shared_ptr<BackgroundImage> tmp = m_Backgrounds[i];
        if (camera_movement_dis >= m_PositionX[i]) {
            tmp->SetVisible(true);
            tmp->SetPosition(620.0f,m_PositionY[i]);
        }else {
            return i;
        }
    }
}**/
