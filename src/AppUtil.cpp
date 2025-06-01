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
    // if(m_Phase == Phase::Level1_2) {
    //     m_Mario->SetPosition({-380.0f + 3.0f * BLOCK_SIZE, -240.0f + 7.5 * BLOCK_SIZE});
    // }else {
    m_Mario->SetPosition(-380.0f + 2.5f * BLOCK_SIZE, -232.0f);
    //}
    m_Mario->SetImages({RESOURCE_DIR"/Entities/mario_stand.png"}, 1000, 0);
    m_Mario->m_Transform.scale = {MARIO_MAGNIFICATION, MARIO_MAGNIFICATION};
    m_Mario->ResetStateForNextPhase();

    // update next level block
    m_Mario->ClearCollisionBoxes();
    m_Mario->ClearCollisionBlocks();

    // remove last level and set next level items
    std::vector<std::shared_ptr<Props>> tmp2 = m_PM->GetProps();
    for (const auto & prop : tmp2) {
        std::shared_ptr<Util::GameObject> tmp3 = prop;
        m_Root.RemoveChild(tmp3);
    }

    std::vector<std::shared_ptr<Fireball>> tmp4 = m_FM->GetFireballs();
    for (const auto & fireball : tmp4) {
        std::shared_ptr<Util::GameObject> tmp3 = fireball;
        m_Root.RemoveChild(tmp3);
    }
    m_FM->ResetFireballs();

    std::vector<std::shared_ptr<BackgroundImage>> tmp = m_BM->GetBackground();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp3 = img;
        m_Root.RemoveChild(tmp3);
    }

    // remove old enemy
    std::vector<std::shared_ptr<Enemy>> ftmp = m_EM->GetEnemies();
    for (const auto & img : ftmp) {
        std::shared_ptr<Util::GameObject> ftmp2 = img;
        m_Root.RemoveChild(ftmp2);
    }

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
        std::shared_ptr<Props> prop = nullptr;
        std::shared_ptr<Props> prop2 = nullptr;
        Block::PROP_TYPE prop_type = Block::PROP_TYPE::None;
        Block::PROP_TYPE prop_type2 = Block::PROP_TYPE::None;

        // Block with props(mystery block)
        if (hasProp) {
            int prop_imgidx = propsidx[propidx++];
            auto mysteryBlock = std::make_shared<MysteryBlock>();

            switch (prop_imgidx) {
                case 0: {
                    prop = std::make_shared<OneUpMushroom>();
                    prop->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                    prop_type = Block::PROP_TYPE::OneUpMushroom;
                    prop_type2 = Block::PROP_TYPE::None;
                    break;
                }

                case 1: {
                    prop = std::make_shared<MagicMushroom>();
                    prop->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                    prop_type = Block::PROP_TYPE::MagicMushroom;

                    int prop_imgidx2 = 0;
                    if (m_Phase == Phase::Level1_2) {
                        prop_imgidx2 = 12;
                    } else {
                        prop_imgidx2 = 8;
                    }
                    prop2 = std::make_shared<FireFlower>();
                    prop2->SetImage({
                                        m_PM->propsImagePaths[prop_imgidx2],
                                        m_PM->propsImagePaths[prop_imgidx2 + 1],
                                        m_PM->propsImagePaths[prop_imgidx2 + 2],
                                        m_PM->propsImagePaths[prop_imgidx2 + 3]
                                    }, 1000, 0);
                    prop_type2 = Block::PROP_TYPE::FireFlower;
                    break;
                }
                case 2: {
                    prop = std::make_shared<Starman>();
                    prop->SetImage({
                                       m_PM->propsImagePaths[prop_imgidx],
                                       m_PM->propsImagePaths[prop_imgidx + 1],
                                       m_PM->propsImagePaths[prop_imgidx + 2],
                                       m_PM->propsImagePaths[prop_imgidx + 3],
                                       m_PM->propsImagePaths[prop_imgidx + 4],
                                       m_PM->propsImagePaths[prop_imgidx + 5]
                                   }, 200, 0);
                    prop_type = Block::PROP_TYPE::Starman;
                    prop_type2 = Block::PROP_TYPE::None;
                    break;
                }

                case 8:
                case 11: {
                    // Won't be used here(only used when mystery block only has fire flower)
                    prop = std::make_shared<FireFlower>();
                    prop->SetImage({
                                       m_PM->propsImagePaths[prop_imgidx],
                                       m_PM->propsImagePaths[prop_imgidx + 1],
                                       m_PM->propsImagePaths[prop_imgidx + 2],
                                       m_PM->propsImagePaths[prop_imgidx + 3]
                                   }, 1000, 0);
                    prop_type = Block::PROP_TYPE::FireFlower;
                    prop_type2 = Block::PROP_TYPE::None;
                    break;
                }

                case 16: {
                    prop = std::make_shared<Coin>();
                    prop->SetImage({
                                       m_PM->propsImagePaths[prop_imgidx],
                                       m_PM->propsImagePaths[prop_imgidx + 1],
                                       m_PM->propsImagePaths[prop_imgidx + 2],
                                       m_PM->propsImagePaths[prop_imgidx + 3]
                                   }, 200, 0);
                    prop_type = Block::PROP_TYPE::Coin;
                    prop_type2 = Block::PROP_TYPE::None;
                    break;
                }

                default:
                    continue; // Unsupported item type
            }

            prop->SetScale(PROP_MAGNIFICATION, PROP_MAGNIFICATION);
            prop->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            prop->SetZIndex(-30);
            props.push_back(prop);

            mysteryBlock->SetInsidePropType(prop_type, prop_type2);
            mysteryBlock->SetChooseProp(true);
            if (prop_type == Block::PROP_TYPE::MagicMushroom) {
                prop2->SetScale(PROP_MAGNIFICATION, PROP_MAGNIFICATION);
                prop2->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
                prop2->SetZIndex(-30);
                props.push_back(prop2);
            }
            mysteryBlock->SetProps(prop, prop2);

            // Special case: Level 1-2 specific coordinates coin can be triggered 7 times
            if (m_Phase == Phase::Level1_2 && ((tmpx[i] == 29 || tmpx[i] == 73) && tmpy[i] == 6)) mysteryBlock->SetCollisionTime(7);

            block = mysteryBlock;
        }else {
            // Block without prop
            if (tmpidx[i] == 0 || tmpidx[i] == 1) {
                auto commonBlock = std::make_shared<CommonBlock>();
                block = commonBlock;
            } else {
                auto immovableBlock = std::make_shared<ImmovableBlock>();
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
            // inside koopa setting
            auto inside_koopa = std::make_shared<Koopa>();
            inside_koopa->SetLive(0);
            inside_koopa->SetImage({m_EM->imageFiles[20]}, 1000, 0);
            inside_koopa->SetScale(ENEMY_MAGNIFICATION,ENEMY_MAGNIFICATION);
            inside_koopa->SetPosition(ftmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET,ftmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            inside_koopa->SetVisible(false);
            eneimes.push_back(inside_koopa);
            if (m_Phase == Phase::Level1_2) eneimes.back()->SetOverworld(false);

            auto flykoopa = std::make_shared<FlyKoopa>();
            flykoopa->SetInsideKoopa(inside_koopa);
            eneimes.push_back(flykoopa);
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
                m_Phase = Phase::Start;
                break;
            default:
                break;
        }
    }else {
        m_Mario->SetGrow(false);
        m_Mario->SetInvincible(false);
        m_Mario->SetFire(false);
    }
    switch (m_Phase) {
        case Phase::Start:
            ResetPhase();
            // score <= 0
            m_Mario->IncreaseScore(-m_Mario->GetScore());
            // coin <= 0
            m_Mario->IncreaseCoin(-m_Mario->GetCoin());
            m_Mario->SetFire(false);
            m_Mario->SetGrow(false);
            m_Mario->SetInvincible(false);
            m_Mario->SetLive(3);
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
            m_BGM->Play();
            break;
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
            m_EM->SetupEnemyCollisions();
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
            m_EM->SetupEnemyCollisions();
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
            m_EM->SetupEnemyCollisions();
            break;
        default:
            break;
    }
}