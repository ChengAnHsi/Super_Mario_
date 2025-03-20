//
// Created by 112590007 on 2025/3/14.
//

#ifndef BLOCKMANAGER_HPP
#define BLOCKMANAGER_HPP

#include "BackgroundImage.hpp"

class BlockManger {
public:
    BlockManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (int i = 0; i < m_Backgrounds.size(); i++) {
            all_obj.emplace_back(m_Backgrounds[i]);
        }
        return all_obj;
    }

    //int block_visible(float camera_movement_dis, int idx);
private:
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
    // map 1-1
    std::vector<int> tmp_x = {22,80,81,82,83,84,85,86,87,91,92,93,94,109,121,122,123,128,129,130,131,188,189,
        187,188,189,
        186,187,188,189,
        64,185,186,187,188,189,
        16,20,21,22,23,24,77,78,79,94,100,101,106,109,112,118,129,130,137,140,151,152,155,168,169,170,171,184,185,186,187,188,189,
        136,137,140,141,150,151,152,155,156,183,184,185,186,187,188,189,
        135,136,137,140,141,142,149,150,151,152,155,156,157,182,183,184,185,186,187,188,189,
        134,135,136,137,140,141,142,143,148,149,150,151,152,155,156,157,158,181,182,183,184,185,186,187,188,189};
    std::vector<int> tmp_y = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
        8,8,8,
        7,7,7,7,
        6,6,6,6,6,6,
        5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
    std::vector<int> imgidx = {6,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,6,6,0,4,4,
        4,4,4,
        4,4,4,4,
        6,4,4,4,4,4,
        6,0,6,0,6,0,0,6,0,0,0,0,6,6,6,0,0,0,4,4,4,4,4,0,0,6,0,4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};

    std::vector<std::shared_ptr<BackgroundImage>> m_Backgrounds;
    std::vector<float> m_PositionX;
    std::vector<float> m_PositionY;
};

#endif //BLOCKMANAGER_HPP
