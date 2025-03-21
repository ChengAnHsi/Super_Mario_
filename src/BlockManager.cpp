//
// Created by 112590007 on 2025/3/14.
//
#include "BlockManager.hpp"
#include "Global.hpp"

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
        // correct x position or cut small floor to solve align and hole
        // x: i * BLOCK_SIZE + (22.0f * i / 8)
        // or: i * BLOCK_SIZE - 380.0f
        m_PositionX.push_back(i * BLOCK_SIZE - 380.0f);
        // correct y position subtract 25 more
        m_PositionY.push_back(BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Scenery/Overworld/floorbricks.png");
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(m_PositionX.back(),m_PositionY.back());

        m_PositionX.push_back(i * BLOCK_SIZE - 380.0f);
        // correct y position subtract 25 more
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
