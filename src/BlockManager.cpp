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

#include "FireFlower.hpp"
#include "MagicMushroom.hpp"
#include "OneUpMushroom.hpp"
#include "Starman.hpp"

BlockManager::BlockManager() {
    int imgidx_size = imgidx.size();
    int propsidx = 0;
    for (size_t i = 0; i < imgidx_size; i++) {
        m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
        m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        // TODO props setting
        bool setprop = false;
        if (props_tmp_x[propsidx] == tmp_x[i] && props_tmp_y[propsidx] == tmp_y[i]) {
            setprop = true;
        }

        std::shared_ptr<Props> tempp;
        if (setprop) {
            int prop_imgidx = 0;
            prop_imgidx = props_imgidx[propsidx];
            propsidx += 1;

            if (prop_imgidx == 0) {
                tempp = std::make_shared<OneUpMushroom>();
                tempp->SetImage(propsImagePaths[prop_imgidx]);
            }else if (prop_imgidx == 1) {
                tempp = std::make_shared<MagicMushroom>();
                tempp->SetImage(propsImagePaths[prop_imgidx]);
            }else if (prop_imgidx == 2) {
                tempp = std::make_shared<Starman>();
                tempp->SetImage({propsImagePaths[prop_imgidx], propsImagePaths[prop_imgidx+1], propsImagePaths[prop_imgidx+2], propsImagePaths[prop_imgidx+3], propsImagePaths[prop_imgidx+4], propsImagePaths[prop_imgidx+5]}, 1000, 0);
            }else if (prop_imgidx == 8) {
                tempp = std::make_shared<FireFlower>();
                tempp->SetImage({propsImagePaths[prop_imgidx], propsImagePaths[prop_imgidx+1], propsImagePaths[prop_imgidx+2], propsImagePaths[prop_imgidx+3], propsImagePaths[prop_imgidx+4], propsImagePaths[prop_imgidx+5]}, 1000, 0);
            }
            tempp->SetScale(BLOCK_MAGNIFICATION - 1, BLOCK_MAGNIFICATION - 1);
            tempp->SetPosition(m_PositionX[i],m_PositionY[i]);
            // tempp->SetZIndex(-30);
        }
        if(imgidx[i] == 6 || imgidx[i] == 9) {
            auto temp = std::make_shared<MysteryBlock>();
            if (setprop) {
                temp->SetProps(tempp);
            }
            m_Blocks.push_back(temp);
            m_Blocks.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i] + 1],imagePaths[imgidx[i] + 2]}, 1000, 0);
        }else if(imgidx[i] == 0 || imgidx[i] == 1) {
            auto temp = std::make_shared<CommonBlock>();
            if (setprop) {
                temp->SetProps(tempp);
            }
            m_Blocks.push_back(temp);
            m_Blocks.back()->SetImage(imagePaths[imgidx[i]]);
        }else {
            m_Blocks.push_back(std::make_shared<ImmovableBlock>());
            m_Blocks.back()->SetImage(imagePaths[imgidx[i]]);
        }
        m_Blocks.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        m_Blocks.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
    }
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

std::vector<int> BlockManager::GetpropsX(int phase){
    switch (phase)
    {
        case 1:
            return props_tmp_x;
        case 2:
            return props_tmp_x2;
        case 3:
            return props_tmp_x3;
        default:
            return props_tmp_x;
    }
}

std::vector<int> BlockManager::GetpropsY(int phase){
    switch (phase)
    {
        case 1:
            return props_tmp_y;
        case 2:
            return props_tmp_y2;
        case 3:
            return props_tmp_y3;
        default:
            return props_tmp_y;
    }
}

std::vector<int> BlockManager::Getpropsidx(int phase){
    switch (phase)
    {
        case 1:
            return props_imgidx;
        case 2:
            return props_imgidx2;
        case 3:
            return props_imgidx3;
        default:
            return props_imgidx;
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

void BlockManager::UpdatePropsAnimation() {
    for (int i = 0; i < m_Blocks.size(); i++) {
        if (m_Blocks[i]->GetBlocktype() == Block::TYPE::CommonBlock || m_Blocks[i]->GetBlocktype() == Block::TYPE::MysteryBlock) {
            auto block = m_Blocks[i];
            auto common = std::dynamic_pointer_cast<CommonBlock>(block);
            if (common) {
                auto prop = common->GetProps();
                if (prop) {
                    prop->Update(1.0f);
                }
            }

            auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block);
            if (mystery) {
                auto prop = mystery->GetProps();
                if (prop) {
                    prop->Update(1.0f);
                }
            }
        }
    }
}

void BlockManager::AddAllPropsCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (int i = 0; i < m_Blocks.size(); i++) {
        if (m_Blocks[i]->GetBlocktype() == Block::TYPE::CommonBlock || m_Blocks[i]->GetBlocktype() == Block::TYPE::MysteryBlock) {
            auto block = m_Blocks[i];
            auto common = std::dynamic_pointer_cast<CommonBlock>(block);
            if (common) {
                auto prop = common->GetProps();
                if (prop) {
                    prop->AddCollisionBlocks(blocks);
                }
            }

            auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block);
            if (mystery) {
                auto prop = mystery->GetProps();
                if (prop) {
                    prop->AddCollisionBlocks(blocks);
                }
            }
        }
    }
}

void BlockManager::AddAllPropsCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (int i = 0; i < m_Blocks.size(); i++) {
        auto block = m_Blocks[i];
        auto common = std::dynamic_pointer_cast<CommonBlock>(block);
        if (common) {
            auto prop = common->GetProps();
            if (prop) {
                prop->AddCollisionBoxes(boxes);
            }
        }

        auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block);
        if (mystery) {
            auto prop = mystery->GetProps();
            if (prop) {
                prop->AddCollisionBoxes(boxes);
            }
        }
    }
}