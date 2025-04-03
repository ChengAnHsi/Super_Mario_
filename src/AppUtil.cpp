#include "AppUtil.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "AnimatedImage.hpp"
#include "Util/Logger.hpp"

// update all game object for next level
void App::NextPhase() {
    if(m_Phase == Phase::Level1_2) {
        m_Mario->SetPosition({-380.0f + 3.0f * BLOCK_SIZE, -240.0f + 9.5 * BLOCK_SIZE});
    }
    // update next level block
    m_Mario->ClearCollisionBox();

    // remove last level and set next level block
    std::vector<std::shared_ptr<BackgroundImage>> tmp = m_BM->GetBackground();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }

    // add new block to render
    std::vector<std::shared_ptr<BackgroundImage>> backgrounds;
    std::vector tmpx = m_BM->GetX(static_cast<int>(m_Phase));
    std::vector tmpy = m_BM->GetY(static_cast<int>(m_Phase));
    std::vector tmpidx = m_BM->Getidx(static_cast<int>(m_Phase));

    int imgidx_size = tmpidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        if(tmpidx[i] == 6 || tmpidx[i] == 9) {
            backgrounds.push_back(std::make_shared<AnimatedImage>());
            backgrounds.back()->SetImage({m_BM->imagePaths[tmpidx[i]],m_BM->imagePaths[tmpidx[i] + 1],m_BM->imagePaths[tmpidx[i] + 2]}, 1000, 0);
        }else {
            backgrounds.push_back(std::make_shared<BackgroundImage>());
            backgrounds.back()->SetImage(m_BM->imagePaths[tmpidx[i]]);
        }
        backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        backgrounds.back()->SetPosition(tmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET,tmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
    }
    m_BM->SetBackground(backgrounds);
    m_Mario->AddCollisionBox(backgrounds);
    m_Root.AddChildren(m_BM->GetChildren());

    // remove old enemy
    std::vector<std::shared_ptr<BackgroundImage>> ftmp = m_EM->GetBackground();
    for (const auto & img : ftmp) {
        std::shared_ptr<Util::GameObject> ftmp2 = img;
        m_Root.RemoveChild(ftmp2);
    }

    // add new enemy to render
    std::vector<float> ftmpx = m_EM->GetX(static_cast<int>(m_Phase));
    std::vector<float> ftmpy = m_EM->GetY(static_cast<int>(m_Phase));
    std::vector ftmpidx = m_EM->Getidx(static_cast<int>(m_Phase));
    backgrounds.clear();
    int fimgidx_size = ftmpidx.size();
    for (int i = 0; i < fimgidx_size; i++) {
        backgrounds.push_back(std::make_shared<BackgroundImage>());
        backgrounds.back()->SetImage(m_EM->imageFiles[ftmpidx[i]]);
        backgrounds.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        backgrounds.back()->SetPosition(ftmpx[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, ftmpy[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
    }
    m_EM->SetBackground(backgrounds);
    m_Root.AddChildren(m_EM->GetChildren());

    // remove tube and other things
    // The castle and flag are removed and use the updated new locations
    tmp = m_PRM->GetBackground();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }
    tmp = m_PRM->GetTube();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp2 = img;
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
            m_Mario->AddCollisionBox(m_PRM->GetTube());
            break;
        case Phase::Level1_1:
            LOG_DEBUG("Congratulations! You have completed Level1-1!");
            m_Phase = Phase::Level1_2;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase), m_BGM);
            m_Mario->AddCollisionBox(m_PRM->GetTube());
            m_Root.AddChildren(m_PRM->GetChildren(false));
            break;
        case Phase::Level1_2:
            LOG_DEBUG("Congratulations! You have completed Level1-2!");
            m_Phase = Phase::Level1_3;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase), m_BGM);
            m_Mario->AddCollisionBox(m_PRM->GetTube());
            m_Root.AddChildren(m_PRM->GetChildren(false));
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
