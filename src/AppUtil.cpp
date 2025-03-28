#include "AppUtil.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "Util/Logger.hpp"

// update all game object for next level
void App::NextPhase() {
    // update next level block
    // remove last level and set next level block
    std::vector<std::shared_ptr<BackgroundImage>> tmp = m_BM->GetBackground();
    for (const auto & img : tmp) {
        std::shared_ptr<Util::GameObject> tmp2 = img;
        m_Root.RemoveChild(tmp2);
    }

    // add new block to render
    std::vector tmpx = m_BM->GetX(static_cast<int>(m_Phase));
    std::vector tmpy = m_BM->GetY(static_cast<int>(m_Phase));
    std::vector tmpidx = m_BM->Getidx(static_cast<int>(m_Phase));
    std::vector<std::shared_ptr<BackgroundImage>> backgrounds;
    int imgidx_size = tmpidx.size();
    for (int i = 0; i < imgidx_size; i++) {
        backgrounds.push_back(std::make_shared<BackgroundImage>());
        backgrounds.back()->ChangeImg(m_BM->imageFiles[tmpidx[i]]);
        backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        backgrounds.back()->SetPosition(tmpx[i] * BLOCK_SIZE - 335.0f,tmpy[i] * BLOCK_SIZE - 325.0f);
    }
    m_BM->SetBackground(backgrounds);
    m_Root.AddChildren(m_BM->GetChildren());
     std::vector<std::shared_ptr<BackgroundImage>> ftmp = m_EM->GetBackground();
    for (const auto & img : ftmp) {
        std::shared_ptr<Util::GameObject> ftmp2 = img;
        m_Root.RemoveChild(ftmp2);
    }

    // add new block to render
    std::vector<float> ftmpx = m_EM->GetX(static_cast<int>(m_Phase));
    std::vector<float> ftmpy = m_EM->GetY(static_cast<int>(m_Phase));
    std::vector ftmpidx = m_EM->Getidx(static_cast<int>(m_Phase));
    backgrounds.clear();
    int fimgidx_size = ftmpidx.size();
    for (int i = 0; i < fimgidx_size; i++) {
        backgrounds.push_back(std::make_shared<BackgroundImage>());
        backgrounds.back()->ChangeImg(m_EM->imageFiles[ftmpidx[i]]);
        backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        backgrounds.back()->SetPosition(ftmpx[i] * BLOCK_SIZE - 335.0f,ftmpy[i] * BLOCK_SIZE - 325.0f);
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
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            break;
        case Phase::Level1_1:
            LOG_DEBUG("Congratulations! You have completed Level1-1!");
            m_Phase = Phase::Level1_2;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            m_Root.AddChildren(m_PRM->GetChildren(false));
            break;
        case Phase::Level1_2:
            LOG_DEBUG("Congratulations! You have completed Level1-2!");
            m_Phase = Phase::Level1_3;
            NextPhase();
            m_PRM->NextPhase(static_cast<int>(m_Phase));
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
