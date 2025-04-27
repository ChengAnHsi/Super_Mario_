//
// Created by 112590007 on 2025/3/14.
//

#include "BlockManager.hpp"

#include "Block.hpp"
#include "MysteryBlock.hpp"
#include "CommonBlock.hpp"
#include "BackgroundImage.hpp"
#include "Global.hpp"
#include "App.hpp"

BlockManager::BlockManager() {
    // initial screen floor
    for(int j = 1; j >= 0; j--) {
        for (int i = 0; i < 15; i++) {
            auto temp = std::make_shared<CommonBlock>();
            m_Blocks.push_back(temp);
            m_Blocks.back()->SetImage(imagePaths[2]);

            m_Blocks.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
            m_Blocks.back()->SetPosition(i * BLOCK_SIZE + BACKGROUND_X_OFFSET, j * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        }
    }
}

std::vector<std::shared_ptr<Util::GameObject>> BlockManager::GetChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
    for (auto block : m_Blocks) {
        if (const auto common = std::dynamic_pointer_cast<CommonBlock>(block)) {
            if (auto prop = common->GetProps()) {
                all_obj.push_back(prop);
            }
        }

        if (const auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block)) {
            if (auto prop = mystery->GetProps()) {
                all_obj.push_back(prop);
            }
        }

        all_obj.push_back(std::static_pointer_cast<Util::GameObject>(block));
    }
    return all_obj;
}

std::vector<int> BlockManager::GetX(int phase){
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
    m_Blocks.clear();
    m_Blocks = std::move(blocks);
}

std::vector<std::shared_ptr<BackgroundImage>> BlockManager::GetBackground() const {
    std::vector<std::shared_ptr<BackgroundImage>> all_block;
    for (const auto & m_Block : m_Blocks) {
        all_block.push_back(std::static_pointer_cast<BackgroundImage>(m_Block));
    }
    return all_block;
}

std::vector<std::shared_ptr<Block>> BlockManager::GetBlocks(){
    return m_Blocks;
}

void BlockManager::UpdatePropsAnimation() const {
    for (const auto & m_Block : m_Blocks) {
        if (m_Block->GetBlocktype() == Block::TYPE::CommonBlock || m_Block->GetBlocktype() == Block::TYPE::MysteryBlock) {
            const auto& block = m_Block;
            if (const auto common = std::dynamic_pointer_cast<CommonBlock>(block)) {
                if (const auto prop = common->GetProps()) {
                    prop->Update(1.0f);
                }
            }

            if (const auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block)) {
                if (const auto prop = mystery->GetProps()) {
                    prop->Update(1.0f);
                }
            }
        }
    }
}

void BlockManager::AddAllPropsCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) const {
    for (const auto & m_Block : m_Blocks) {
        if (m_Block->GetBlocktype() == Block::TYPE::CommonBlock || m_Block->GetBlocktype() == Block::TYPE::MysteryBlock) {
            const auto& block = m_Block;
            if (const auto common = std::dynamic_pointer_cast<CommonBlock>(block)) {
                if (const auto prop = common->GetProps()) {
                    prop->AddCollisionBlocks(blocks);
                }
            }

            if (const auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block)) {
                if (const auto prop = mystery->GetProps()) {
                    prop->AddCollisionBlocks(blocks);
                }
            }
        }
    }
}

void BlockManager::AddAllPropsCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) const {
    for (const auto& block : m_Blocks) {
        if (const auto common = std::dynamic_pointer_cast<CommonBlock>(block)) {
            if (const auto prop = common->GetProps()) {
                prop->AddCollisionBoxes(boxes);
            }
        }

        if (const auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block)) {
            if (const auto prop = mystery->GetProps()) {
                prop->AddCollisionBoxes(boxes);
            }
        }
    }
}
