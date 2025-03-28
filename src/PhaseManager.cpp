#include "BlockManager.hpp"
#include "PhaseResourceManger.hpp"
#include "App.hpp"
#include "Global.hpp"
#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    last_update = std::chrono::steady_clock::now();

    m_MarioText = std::make_shared<TaskText>();
    m_MarioText->SetPosition(-250, 300);
    m_MarioText->SetTxtIdx(1, 0);

    m_MoneyText = std::make_shared<TaskText>();
    m_MoneyText->SetPosition(-70, 285);
    m_MoneyText->SetTxtIdx(2, 0);

    m_WorldText = std::make_shared<TaskText>();
    m_WorldText->SetPosition(100, 300);
    m_WorldText->SetTxtIdx(3, 1);

    m_TimeText = std::make_shared<TaskText>();
    m_TimeText->SetPosition(300, 300);
    m_TimeText->SetTxtIdx(4, 400);

    m_OtherText = std::make_shared<TaskText>();
    m_OtherText->SetPosition(0, -200);
    m_OtherText->SetTxtIdx(0, 0);

    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Scenery/Overworld/bush1.png",
        RESOURCE_DIR"/Scenery/Overworld/bush2.png",
        RESOURCE_DIR"/Scenery/Overworld/cloud1.png",
        RESOURCE_DIR"/Scenery/Overworld/cloud2.png",
        RESOURCE_DIR"/Scenery/Overworld/mountain1.png",
        RESOURCE_DIR"/Scenery/Overworld/mountain2.png",
        RESOURCE_DIR"/Scenery/castle.png",
        RESOURCE_DIR"/Scenery/flag-mast.png"
    };

    // background image(logo or sky)
    // start set
    // [0]: logo
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetPosition(0, 0);
    m_Background.back()->SetSize(3.0f,3.0f);
    // The background z-index is placed at the bottom
    m_Background.back()->SetZIndex(-50);
    // [1]: start mountain(left)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->ChangeImg(imageFiles[5]);
    m_Background.back()->SetPosition(-240.0f, 2.5 * BLOCK_SIZE - 315.0f);
    m_Background.back()->SetSize(1.5f,1.5f);
    // [2]: start bush(right)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->ChangeImg(imageFiles[0]);
    m_Background.back()->SetPosition(11 * BLOCK_SIZE - 360.0f, 2 * BLOCK_SIZE- 325.0f);
    m_Background.back()->SetSize(1.3f,1.3f);

    // tube init(map 1-1)
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-small-tube.png");
    m_Tube.back()->SetPosition(28 * BLOCK_SIZE - 320.0f, 3 * BLOCK_SIZE  - 357.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-medium-tube.png");
    m_Tube.back()->SetPosition(38 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE  - 385.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-large-tube.png");
    m_Tube.back()->SetPosition(46 * BLOCK_SIZE - 320.0f, 5 * BLOCK_SIZE  - 415.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-large-tube.png");
    m_Tube.back()->SetPosition(57 * BLOCK_SIZE - 320.0f, 5 * BLOCK_SIZE  - 415.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-small-tube.png");
    m_Tube.back()->SetPosition(163 * BLOCK_SIZE - 320.0f, 3 * BLOCK_SIZE  - 357.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
    m_Tube.push_back(std::make_shared<BackgroundImage>());
    m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-small-tube.png");
    m_Tube.back()->SetPosition(179 * BLOCK_SIZE - 320.0f, 3 * BLOCK_SIZE  - 357.0f);
    m_Tube.back()->SetSize(2.5f, 2.5f);
}

void PhaseResourceManger::NextPhase(int m_Phase) {
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);

    if (m_Phase == 1){
        // [0]: blue or black background image
        m_Background[0]->NextPhase(m_Phase);
        m_Background[0]->SetSize(80.0f,7.0f);
        m_Background[0]->SetZIndex(-50);
        // [1]: castle is visible
        // map 1-1 castle
        m_Background[1]->ChangeImg(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(202 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE - 325.0f);
        m_Background[1]->SetSize(3.0f, 3.0f);
        // [2]: flag set
        m_Background[2]->ChangeImg(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(198 * BLOCK_SIZE - 320.0f, 8 * BLOCK_SIZE - 390.0f);
        m_Background[2]->SetSize(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        // [3]> mountain, cloud... set invisible use app->nextphase
    }
    if (m_Phase == 2){
        m_Background.clear();
        m_Tube.clear();
        // other level tube and its position

        // test successful
        /**m_Tube.push_back(std::make_shared<BackgroundImage>());
        m_Tube.back()->ChangeImg(RESOURCE_DIR"/Scenery/vertical-small-tube.png");
        m_Tube.back()->SetPosition(28 * BLOCK_SIZE - 320.0f, 3 * BLOCK_SIZE  - 357.0f);
        m_Tube.back()->SetSize(2.5f, 2.5f);**/
    }
    if (m_Phase == 3) {
        m_Background.clear();
        m_Tube.clear();
    }

    SetTime(LEVEL_TIME[m_Phase]);
    m_WorldText->SetTxtIdx(3, m_Phase);
    m_OtherText->SetTxtIdx(5, 0);
}

void PhaseResourceManger::ResetPosition() const {
    m_MarioText->SetPosition(-250, 300);
    m_MoneyText->SetPosition(-70, 285);
    m_WorldText->SetPosition(100, 300);
    m_TimeText->SetPosition(300, 300);
}

std::vector<std::shared_ptr<BackgroundImage> > PhaseResourceManger::GetTube() {
    return m_Tube;
}

std::vector<std::shared_ptr<BackgroundImage> > PhaseResourceManger::GetBackground() {
    return m_Background;
}

void PhaseResourceManger::DecreaseTime() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - last_update;

    if (elapsed.count() >= 1.0f) { // 每秒執行一次
        last_update = now;
        time -= 1;

        if (time < 0) time = 0;
        m_TimeText->SetTxtIdx(4, time);
    }
}

void PhaseResourceManger::SetTime(const int time) {
    this->time = time;
}

int PhaseResourceManger::GetTime() const {
    return time;
}