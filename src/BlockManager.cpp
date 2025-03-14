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
    std::vector tmp_x = {22,80,81,82,83,84,85,86,87,91,92,93,94,109,121,122,123,128,129,130,131,188,189,
        187,188,189,
        186,187,188,189,
        64,185,186,187,188,189,
    16,20,21,22,23,24,77,78,79,94,100,101,106,109,112,118,129,130,137,140,151,152,155,168,169,170,171,184,185,186,187,188,189,
    136,137,140,141,150,151,152,155,184,185,186,187,188,189,
    135,136,137,140,141,142,149,150,151,152,155,156,157,182,183,184,185,186,187,188,189};
    std::vector tmp_y = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
        8,8,8,
        7,7,7,7,
        6,6,6,6,6,6,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
    std::vector imgidx = {6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,4,4,
        4,4,4,
        4,4,4,4,
        6,4,4,4,4,4,
    6,0,6,0,6,0,0,6,0,0,0,0,6,6,6,0,0,0,4,4,4,4,4,0,0,6,0,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,0,0,6,0,4,4,4,4,4,4,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

    for (int i = 0; i < imgidx.size();i++) {
        // position should change to correct position
        m_PositionX.push_back(tmp_x[i] * 30);
        m_PositionY.push_back(tmp_y[i] * 30 - 150.0f);
        m_Backgrounds.push_back(std::make_shared<BackgroundImage>());
        m_Backgrounds[i]->ChangeImg(imageFiles[imgidx[i]]);
        m_Backgrounds[i]->SetSize(2.0f,2.0f);
        m_Backgrounds[i]->SetVisible(false);
        is_visible.push_back(false);
    }

}

void BlockManger::block_visible(float camera_movement_dis) {
    for (int i = 0; i < m_Backgrounds.size();i++) {
        std::shared_ptr<BackgroundImage> tmp = m_Backgrounds[i];
        if (camera_movement_dis >= m_PositionX[i] and is_visible[i]== false) {
            tmp->SetVisible(true);
            tmp->SetPosition(600.0f,m_PositionY[i]);
            is_visible[i] = true;
        }
        /**if (camera_movement_dis >= m_PositionX[i] + 600) {
            tmp->SetVisible(false);
        }**/
    }

}
