#include "AppUtil.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "MysteryBlock.hpp"
#include "CommonBlock.hpp"
#include "ImmovableBlock.hpp"
#include "Enemy.hpp"
#include "Flower.hpp"
#include "Goomba.hpp"
#include "OneUpMushroom.hpp"
#include "EnemyManager.hpp"
#include "Util/Logger.hpp"

#include "FlyKoopa.hpp"
#include "Koopa.hpp"
#include <iostream>


// update all game object for next level
void App::NextPhase() {
    if(m_Phase == Phase::Level1_2) {
        m_Mario->SetPosition({-380.0f + 3.0f * BLOCK_SIZE, -240.0f + 9.5 * BLOCK_SIZE});
    }
    // update next level block
    m_Mario->ClearCollisionBoxes();
    m_Mario->ClearCollisionBlocks();

    // remove last level and set next level block
    std::vector<std::shared_ptr<BackgroundImage>> tmp = m_BM->GetBackground();
    std::vector<std::shared_ptr<Block>> tmp2 = m_BM->GetBlocks();
    for (const auto & block : tmp2) {
        auto mystery = std::dynamic_pointer_cast<MysteryBlock>(block);
        if (mystery) {
            auto prop = mystery->GetProps();
            std::shared_ptr<Util::GameObject> tmp3 = prop;
            m_Root.RemoveChild(tmp3);
        }
    }
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp3 = img;
        m_Root.RemoveChild(tmp3);
    }

    // add new block to render
    std::vector<std::shared_ptr<BackgroundImage>> backgrounds;
    std::vector<std::shared_ptr<Block>> blocks;
    std::vector tmpx = m_BM->GetX(static_cast<int>(m_Phase));
    std::vector tmpy = m_BM->GetY(static_cast<int>(m_Phase));
    std::vector tmpidx = m_BM->Getidx(static_cast<int>(m_Phase));

    int imgidx_size = tmpidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        if(tmpidx[i] == 6 || tmpidx[i] == 9) {
            auto temp = std::make_shared<MysteryBlock>();
            auto tempp = std::make_shared<OneUpMushroom>();
            // TODO props setting
            tempp->SetImage(RESOURCE_DIR"/Collectibles/oneupmushroom.png");
            tempp->SetScale(BLOCK_MAGNIFICATION - 1, BLOCK_MAGNIFICATION - 1);
            tempp->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET,tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            temp->SetProps(tempp);

            blocks.push_back(temp);
            blocks.back()->SetImage({m_BM->imagePaths[tmpidx[i]],m_BM->imagePaths[tmpidx[i] + 1],m_BM->imagePaths[tmpidx[i] + 2]}, 1000, 0);
        }else if(tmpidx[i] == 0 || tmpidx[i] == 1) {
            blocks.push_back(std::make_shared<CommonBlock>());
            blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
        }else {
            blocks.push_back(std::make_shared<ImmovableBlock>());
            blocks.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
        }
        blocks.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        blocks.back()->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET,tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        //backgrounds.emplace_back(blocks.back());
    }
    m_BM->SetBlocks(blocks);
    // m_Mario->AddCollisionBoxes(backgrounds);
    m_Mario->AddCollisionBlocks(blocks);
    m_Root.AddChildren(m_BM->GetChildren());

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
    backgrounds.clear();
    int fimgidx_size = ftmpidx.size();
    for (int i = 0; i < fimgidx_size; i++) {
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

void App::ValidTask() {
    LOG_DEBUG("Validating the task {}", static_cast<int>(m_Phase));
    switch (m_Phase) {
        case Phase::Start:
            LOG_DEBUG("Welcome to Super Mario!");
            m_Phase = Phase::Level1_1;
            // init is Level1_1 don't need to call NextPhase
            m_PRM->NextPhase(static_cast<int>(m_Phase), m_BGM);
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            break;
        case Phase::Level1_1:
            LOG_DEBUG("Congratulations! You have completed Level1-1!");
            m_Phase = Phase::Level1_2;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase), m_BGM);
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            break;
        case Phase::Level1_2:
            LOG_DEBUG("Congratulations! You have completed Level1-2!");
            m_Phase = Phase::Level1_3;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase), m_BGM);
            m_Mario->AddCollisionBoxes(m_PRM->GetCollisionBoxes());
            m_Mario->AddCollectibles(m_PRM->GetCollectibleCoins());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
            m_EM->SetAllEnemyCollisionBoxs(m_PRM->GetCollisionBoxes());
            break;
        case Phase::Level1_3:
            LOG_DEBUG("Congratulations! You have completed Super Mario!");
            m_CurrentState = State::END;
            break;
        default:
            break;
        /**
            if (m_Giraffe->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/giraffe.png")
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(true); });
            std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(false); });**/
    }
}