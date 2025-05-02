#include <iostream>

#include "AppUtil.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "Util/Logger.hpp"

#include "Blocks/MysteryBlock.hpp"
#include "Blocks/CommonBlock.hpp"
#include "Blocks/ImmovableBlock.hpp"

#include "Props/FireFlower.hpp"
#include "Props/MagicMushroom.hpp"
#include "Props/OneUpMushroom.hpp"
#include "Props/Starman.hpp"
#include "Props/Coin.hpp"

#include "Enemies/Enemy.hpp"
#include "Enemies/Goomba.hpp"
#include "Enemies/Flower.hpp"
#include "Enemies/FlyKoopa.hpp"
#include "Enemies/Koopa.hpp"

#include "Manager/EnemyManager.hpp"

// update all game object for next level
void App::ResetPhase() {
    // update mario spawn position
    if(m_Phase == Phase::Level1_2) {
        m_Mario->SetPosition({-380.0f + 3.0f * BLOCK_SIZE, -240.0f + 9.5 * BLOCK_SIZE});
    }else {
        m_Mario->SetPosition({-380.0f + 2.5f * BLOCK_SIZE, -232.0f});
    }

    // update next level block
    m_Mario->ClearCollisionBoxes();
    m_Mario->ClearCollisionBlocks();

    // remove last level and set next level block
    std::vector<std::shared_ptr<Props>> tmp2 = m_PM->GetProps();
    for (const auto & prop : tmp2) {
        std::shared_ptr<Util::GameObject> tmp3 = prop;
        m_Root.RemoveChild(tmp3);
    }

    std::vector<std::shared_ptr<BackgroundImage>> tmp = m_BM->GetBackground();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp3 = img;
        m_Root.RemoveChild(tmp3);
    }

    // add new block to render
    std::vector<std::shared_ptr<Block>> blocks;
    std::vector<std::shared_ptr<Props>> props;
    std::vector tmpx = m_BM->GetX(static_cast<int>(m_Phase));
    std::vector tmpy = m_BM->GetY(static_cast<int>(m_Phase));
    std::vector tmpidx = m_BM->Getidx(static_cast<int>(m_Phase));
    std::vector propsx = m_PM->GetpropsX(static_cast<int>(m_Phase));
    std::vector propsy = m_PM->GetpropsY(static_cast<int>(m_Phase));
    std::vector propsidx = m_PM->Getpropsidx(static_cast<int>(m_Phase));

    size_t imgidx_size = tmpidx.size();
    int propidx = 0;

    for (size_t i = 0; i < imgidx_size; i++) {
        bool hasProp = (propsx[propidx] == tmpx[i] && propsy[propidx] == tmpy[i]);

        std::shared_ptr<Block> block;
        std::shared_ptr<Props> prop;

        // Block with props
        if (hasProp) {
            int prop_imgidx = propsidx[propidx++];
            auto mysteryBlock = std::make_shared<MysteryBlock>();

            switch (prop_imgidx) {
                case 0:
                    prop = std::make_shared<OneUpMushroom>();
                    prop->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                    mysteryBlock->SetInsidePropType(Block::PROP_TYPE::OneUpMushroom);
                    break;

                case 1:
                    prop = std::make_shared<MagicMushroom>();
                    prop->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                    mysteryBlock->SetInsidePropType(Block::PROP_TYPE::MagicMushroom);
                    break;

                case 2:
                    prop = std::make_shared<Starman>();
                    prop->SetImage({
                        m_PM->propsImagePaths[prop_imgidx],
                        m_PM->propsImagePaths[prop_imgidx+1],
                        m_PM->propsImagePaths[prop_imgidx+2],
                        m_PM->propsImagePaths[prop_imgidx+3],
                        m_PM->propsImagePaths[prop_imgidx+4],
                        m_PM->propsImagePaths[prop_imgidx+5]
                    }, 200, 0);
                    mysteryBlock->SetInsidePropType(Block::PROP_TYPE::Starman);
                    break;

                case 8:
                    prop = std::make_shared<FireFlower>();
                    prop->SetImage({
                        m_PM->propsImagePaths[prop_imgidx],
                        m_PM->propsImagePaths[prop_imgidx+1],
                        m_PM->propsImagePaths[prop_imgidx+2],
                        m_PM->propsImagePaths[prop_imgidx+3],
                        m_PM->propsImagePaths[prop_imgidx+4],
                        m_PM->propsImagePaths[prop_imgidx+5]
                    }, 1000, 0);
                    mysteryBlock->SetInsidePropType(Block::PROP_TYPE::FireFlower);
                    break;

                case 16: {
                    prop = std::make_shared<Coin>();
                    prop->SetImage({
                        m_PM->propsImagePaths[prop_imgidx],
                        m_PM->propsImagePaths[prop_imgidx+1],
                        m_PM->propsImagePaths[prop_imgidx+2],
                        m_PM->propsImagePaths[prop_imgidx+3]
                    }, 200, 0);
                    mysteryBlock->SetInsidePropType(Block::PROP_TYPE::Coin);
                    break;
                }

                default:
                    continue; // Unsupported item type
            }

            prop->SetScale(PROP_MAGNIFICATION, PROP_MAGNIFICATION);
            prop->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            prop->SetZIndex(-30);
            props.push_back(prop);

            mysteryBlock->SetProps(prop);

            // Special case: Level 1-2 specific coordinates coin can be triggered 7 times
            if (m_Phase == Phase::Level1_2 &&
                ((tmpx[i] == 29 || tmpx[i] == 73) && tmpy[i] == 6)) {
                mysteryBlock->SetCollisionTime(7);
            }

            block = mysteryBlock;
        }

        // Block without prop
        if (!hasProp) {
            if (tmpidx[i] == 0 || tmpidx[i] == 1) {
                auto commonBlock = std::make_shared<CommonBlock>();
                commonBlock->SetInsidePropType(Block::PROP_TYPE::None);
                block = commonBlock;
            } else {
                auto immovableBlock = std::make_shared<ImmovableBlock>();
                immovableBlock->SetInsidePropType(Block::PROP_TYPE::None);
                block = immovableBlock;
            }
        }

        // Shared Block Settings
        block->SetImage(m_BM->imagePaths[tmpidx[i]]);
        block->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        block->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        blocks.push_back(block);
    }

    m_BM->SetBlocks(blocks);
    m_PM->SetProps(props);
    m_Mario->AddCollisionBlocks(blocks);
    m_Root.AddChildren(m_BM->GetChildren());
    m_Root.AddChildren(m_PM->GetChildren());

    // remove old enemy
    std::vector<std::shared_ptr<Enemy>> ftmp = m_EM->GetEnemies();
    for (const auto & img : ftmp) {
        std::shared_ptr<Util::GameObject> ftmp2 = img;
        m_Root.RemoveChild(ftmp2);
    }

    // add new enemy to render
    std::vector<std::shared_ptr<Enemy>> eneimes;
    std::vector<float> ftmpx = m_EM->GetX(static_cast<int>(m_Phase));
    std::vector<float> ftmpy = m_EM->GetY(static_cast<int>(m_Phase));
    std::vector ftmpidx = m_EM->Getidx(static_cast<int>(m_Phase));
    size_t fimgidx_size = ftmpidx.size();
    for (size_t i = 0; i < fimgidx_size; i++) {
        if(ftmpidx[i] == 2 || ftmpidx[i] == 5){
            eneimes.push_back(std::make_shared<Goomba>());
        }
        else if(ftmpidx[i] == 0 || ftmpidx[i] == 8){
            eneimes.push_back(std::make_shared<Flower>());
        }
        else if(ftmpidx[i] == 10 || ftmpidx[i] == 16 || ftmpidx[i] == 20){
            eneimes.push_back(std::make_shared<Koopa>());
        }
        else if(ftmpidx[i] == 18){
            eneimes.push_back(std::make_shared<FlyKoopa>());
        }
        if (m_Phase == Phase::Level1_2) {
            eneimes.back()->SetOverworld(false);
        }
        eneimes.back()->SetImage({m_EM->imageFiles[ftmpidx[i]]}, 1000, 0);
        eneimes.back()->SetScale(ENEMY_MAGNIFICATION,ENEMY_MAGNIFICATION);
        eneimes.back()->SetPosition(ftmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET,ftmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
    }
    m_EM->SetEnemies(eneimes);
    m_Root.AddChildren(m_EM->GetChildren());

    // remove tube and other things
    // The castle and flag are removed and use the updated new locations
    tmp = m_PRM->GetBackground();
    for (const auto & img : tmp) {
        const std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }
    tmp = m_PRM->GetCollisionBoxes();
    for (const auto & img : tmp) {
        const std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }
    tmp = m_PRM->GetCollectibleCoins();
    for (const auto & img : tmp) {
        const std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }
}

void App::NextPhase(bool is_nextphase) {
    if(is_nextphase) {
        switch (m_Phase) {
            case Phase::Start:
                LOG_DEBUG("Welcome to Super Mario!");
                m_Phase = Phase::Level1_1;
                break;
            case Phase::Level1_1:
                LOG_DEBUG("Congratulations! You have completed Level1-1!");
                m_Phase = Phase::Level1_2;
                break;
            case Phase::Level1_2:
                LOG_DEBUG("Congratulations! You have completed Level1-2!");
                m_Phase = Phase::Level1_3;
                break;
            case Phase::Level1_3:
                LOG_DEBUG("Congratulations! You have completed Super Mario!");
                m_CurrentState = State::END;
                break;
            default:
                break;
        }
    }
    switch (m_Phase) {
        case Phase::Level1_1:
            ResetPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
            m_BGM->Play();
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            m_PM->AddAllPropsCollisionBlocks(m_BM->GetBlocks());
            m_PM->AddAllPropsCollisionBoxes(m_PRM->GetCollisionBoxes());
            break;
        case Phase::Level1_2:
            ResetPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Underworld/theme.mp3");
            m_BGM->Play();
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            m_PM->AddAllPropsCollisionBlocks(m_BM->GetBlocks());
            m_PM->AddAllPropsCollisionBoxes(m_PRM->GetCollisionBoxes());
            break;
        case Phase::Level1_3:
            ResetPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
            m_BGM->Play();
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            m_PM->AddAllPropsCollisionBlocks(m_BM->GetBlocks());
            m_PM->AddAllPropsCollisionBoxes(m_PRM->GetCollisionBoxes());
            break;
        default:
            break;
        /**
            if (m_Giraffe->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/giraffe.png")
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(true); });
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(false); });**/
    }
}