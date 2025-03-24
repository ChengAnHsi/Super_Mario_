#include "AppUtil.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "Util/Logger.hpp"

/**
 * @brief The function to validate the tasks.
 * @warning Do not modify this function.
 * @note See README.md for the task details.
 */

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
        // position should change to correct position
        backgrounds.push_back(std::make_shared<BackgroundImage>());
        backgrounds.back()->ChangeImg(m_BM->imageFiles[tmpidx[i]]);
        backgrounds.back()->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        backgrounds.back()->SetPosition(tmpx[i] * BLOCK_SIZE - 335.0f,tmpy[i] * BLOCK_SIZE - 325.0f);
    }
    m_BM->SetBackground(backgrounds);
    m_Root.AddChildren(m_BM->GetChildren());

    // remove tube and other things
    // The castle and flag are removed and use the updated new locations.
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
            m_PRM->NextPhase(static_cast<int>(m_Phase));
            // init is Level1_1 don't need to call NextPhase
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
            m_CurrentState = State::END;
            //m_Phase = Phase::Level1_3;
            //m_PRM->NextPhase();
            break;
        default:
            break;
        /**case Phase::Level1_3:
            LOG_DEBUG("Congratulations! You have completed Super Mario!");
            m_CurrentState = State::END;
            break;**/

        /**case Phase::CHANGE_CHARACTER_IMAGE:
            if (m_Giraffe->GetImagePath() == GA_RESOURCE_DIR"/Image/Character/giraffe.png") {
                m_Phase = Phase::ABLE_TO_MOVE;
                m_Giraffe->SetPosition({-112.5f, -140.5f});

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The image is not correct");
                LOG_DEBUG("The image path is {} instead.", m_Giraffe->GetImagePath());
            }
            break;

        case Phase::BEE_ANIMATION:
            isBeeLooping = m_Bee->IsLooping();
            isBeePlaying = m_Bee->IsPlaying();

            if (isBeeLooping && isBeePlaying) {
                m_Phase = Phase::OPEN_THE_DOORS;
                m_Giraffe->SetPosition({-112.5f, -140.5f});
                m_Giraffe->SetVisible(true);
                m_Bee->SetVisible(false);
                std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(true); });

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("The bee animation is {} but not {}", isBeeLooping ? "looping" : "playing",
                          isBeeLooping ? "playing" : "looping");
            }
            break;

        case Phase::OPEN_THE_DOORS:
            if (AreAllDoorsOpen(m_Doors)) {
                m_Phase = Phase::COUNTDOWN;
                std::for_each(m_Doors.begin(), m_Doors.end(), [](const auto& door) { door->SetVisible(false); });
                m_Giraffe->SetVisible(false);

                m_PRM->NextPhase();
            } else {
                LOG_DEBUG("At least one door is not open");
            }
            break;**/
    }
}
