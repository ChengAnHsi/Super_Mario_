//
// Created by 112590007 on 2025/3/14.
//
#include "BlockManager.hpp"

BlockManger::BlockManger() {
    int imgidx_size = imgidx.size();
    float BLOCK_SIZE = 45.0f;
    float BLOCK_MAGNIFICATION = 3.0f;
    for (int i = 0; i < imgidx_size; i++) {
        // position should change to correct position
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE - 380.0f);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE - 325.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds[i]->ChangeImg(imageFiles[imgidx[i]]);
        m_Backgrounds[i]->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds[i]->SetPosition(m_PositionX[i],m_PositionY[i]);
    }

    for (int i = 0; i < 224; i++) {
        // correct x position or cut small floor to solve align and hole
        // x: (i - imgidx_size) * BLOCK_SIZE + (22.0f * (i - imgidx_size) / 8)
        m_PositionX.push_back(i * BLOCK_SIZE - 370.0f);
        // correct y position subtract 25 more
        m_PositionY.push_back(BLOCK_SIZE - 350.0f);
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
    m_Backgrounds.back()->SetPosition(16 * BLOCK_SIZE - 380.0f,3 * BLOCK_SIZE - 325.0f);
    m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
    m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
    m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_Backgrounds.back()->SetPosition(16 * BLOCK_SIZE - 380.0f,2 * BLOCK_SIZE - 325.0f);**/

    /**for (int i = 0; i < 16; i++) {
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds.back()->ChangeImg(RESOURCE_DIR"/Blocks/Overworld/block.png");
        m_Backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Backgrounds.back()->SetPosition(i * BLOCK_SIZE,0);
    }**/

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
