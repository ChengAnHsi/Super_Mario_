#include "BlockManager.hpp"
//
// Created by 112590007 on 2025/3/14.
//
BlockManger::BlockManger() {
    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Blocks/Overworld/block.png",
        RESOURCE_DIR"/Blocks/Underworld/block2.png",
        RESOURCE_DIR"/Scenery/Overworld/floorbricks.png",
        RESOURCE_DIR"/Scenery/Underworld/floorbricks.png",
        RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png",
        RESOURCE_DIR"/Blocks/Underworld/immovableBlock.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock0.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock1.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock2.png",
        RESOURCE_DIR"/Blocks/Underworld/misteryBlock0.png",
        RESOURCE_DIR"/Blocks/Underworld/misteryBlock1.png",
        RESOURCE_DIR"/Blocks/Underworld/misteryBlock2.png"
    };

    // 0-1: block 2-3: floor 4-5: immovable block 6-11: mistery block
    std::vector<float> tmp_x = {22,80,81,82,83,84,85,86,87,91,92,93,94,109,121,122,123,128,129,130,131,188,189};
    std::vector<float> tmp_y = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
    std::vector<int> imgidx = {6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,4,4};

    for (int i = 0; i < imgidx.size();i++) {
        // position should change to correct position
        m_PositionX.push_back(tmp_x[i] * 30);
        m_PositionY.push_back(tmp_y[i] * 30);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds[i]->ChangeImg(imageFiles[imgidx[i]]);
        m_Backgrounds[i]->SetSize(2.0f,2.0f);
        m_Backgrounds[i]->SetVisible(false);
    }

}

void BlockManger::block_visible(float camera_movement_dis) {
    for (int i = 0; i < m_Backgrounds.size();i++) {
        std::shared_ptr<BackgroundImage> tmp = m_Backgrounds[i];
        if (camera_movement_dis == m_PositionX[i]) {
            tmp->SetVisible(true);
            tmp->SetPosition(600.0f,m_PositionY[i]);
        }
        /**if (camera_movement_dis >= m_PositionX[i] + 600) {
            tmp->SetVisible(false);
        }**/
    }

}
