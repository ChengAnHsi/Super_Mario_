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
        bool setprop = false;
        if (propsx[propidx] == tmpx[i] && propsy[propidx] == tmpy[i]) {
            setprop = true;
        }

        std::shared_ptr<Props> tempp;
        if (setprop) {
            int prop_imgidx = 0;
            prop_imgidx = propsidx[propidx];
            propidx += 1;

            auto temp = std::make_shared<MysteryBlock>();

            if (prop_imgidx == 0) {
                tempp = std::make_shared<OneUpMushroom>();
                tempp->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                temp->SetInsidePropType(Block::PROP_TYPE::OneUpMushroom);
            }else if (prop_imgidx == 1) {
                tempp = std::make_shared<MagicMushroom>();
                tempp->SetImage(m_PM->propsImagePaths[prop_imgidx]);
                temp->SetInsidePropType(Block::PROP_TYPE::MagicMushroom);
            }else if (prop_imgidx == 2) {
                tempp = std::make_shared<Starman>();
                tempp->SetImage({m_PM->propsImagePaths[prop_imgidx], m_PM->propsImagePaths[prop_imgidx+1], m_PM->propsImagePaths[prop_imgidx+2], m_PM->propsImagePaths[prop_imgidx+3], m_PM->propsImagePaths[prop_imgidx+4], m_PM->propsImagePaths[prop_imgidx+5]}, 200, 0);
                temp->SetInsidePropType(Block::PROP_TYPE::Starman);
            }else if (prop_imgidx == 8) {
                tempp = std::make_shared<FireFlower>();
                tempp->SetImage({m_PM->propsImagePaths[prop_imgidx], m_PM->propsImagePaths[prop_imgidx+1], m_PM->propsImagePaths[prop_imgidx+2], m_PM->propsImagePaths[prop_imgidx+3], m_PM->propsImagePaths[prop_imgidx+4], m_PM->propsImagePaths[prop_imgidx+5]}, 1000, 0);
                temp->SetInsidePropType(Block::PROP_TYPE::FireFlower);
            }else if(prop_imgidx == 16) {
                tempp = std::make_shared<Coin>();
                tempp->SetImage({m_PM->propsImagePaths[prop_imgidx], m_PM->propsImagePaths[prop_imgidx+1], m_PM->propsImagePaths[prop_imgidx+2], m_PM->propsImagePaths[prop_imgidx+3]}, 200, 0);
                temp->SetInsidePropType(Block::PROP_TYPE::Coin);
            }
            tempp->SetScale(PROP_MAGNIFICATION, PROP_MAGNIFICATION);
            tempp->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            // test props position visible or not
            tempp->SetZIndex(-30);
            props.push_back(tempp);


            temp->SetProps(tempp);

            // TODO set coin(collision) 7 times {29,6}, {73,6}
            if (m_Phase == Phase::Level1_2 && ((tmpx[i] == 29 || tmpx[i] == 73) && tmpy[i] == 6)) temp->SetCollisionTime(7);
            blocks.push_back(temp);
            blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
        }

        if(!setprop) {
            if(tmpidx[i] == 6 || tmpidx[i] == 9) {
                // mysteryblock but will not goto here
                // auto temp = std::make_shared<MysteryBlock>();
                // blocks.push_back(temp);
                // blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
            }else if(tmpidx[i] == 0 || tmpidx[i] == 1) {
                // TODO prop drop if common block
                auto temp = std::make_shared<CommonBlock>();
                blocks.push_back(temp);
                blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
                blocks.back()->SetInsidePropType(Block::PROP_TYPE::None);
            }else {
                blocks.push_back(std::make_shared<ImmovableBlock>());
                blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
                blocks.back()->SetInsidePropType(Block::PROP_TYPE::None);
            }
        }

        blocks.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        blocks.back()->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
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