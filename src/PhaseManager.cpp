#include "PhaseResourceManger.hpp"

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
        RESOURCE_DIR"/Scenery/Overworld/castle.png",
        RESOURCE_DIR"/Scenery/Overworld/flag-mast.png"
    };

    // background image(logo or sky)
    // start set
    // [0]: logo and map
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetPosition(0, 0);
    m_Background.back()->SetSize(3.0f,3.0f);
    // The background z-index is placed at the bottom
    m_Background.back()->SetZIndex(-50);
    // start mountain
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->ChangeImg(imageFiles[5]);
    m_Background.back()->SetPosition(-360, -280);
    // start bush
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->ChangeImg(imageFiles[4]);
    m_Background.back()->SetPosition(-360, 3 * 45);
}

void PhaseResourceManger::NextPhase() {
    if (m_Phase == 3) return;
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);
    m_Background[0]->NextPhase(m_Phase);
    m_Background[0]->SetSize(1.8f,1.8f);
    m_Background[0]->SetZIndex(-50);
    for(int i = 1; i < m_Background.size(); i++) {
        m_Background[i]->SetVisible(false);
    }
    m_WorldText->SetTxtIdx(3, m_Phase++);
    m_OtherText->SetTxtIdx(5, 0);
}

void PhaseResourceManger::ResetPosition() const {
    m_MarioText->SetPosition(-250, 300);
    m_MoneyText->SetPosition(-70, 285);
    m_WorldText->SetPosition(100, 300);
    m_TimeText->SetPosition(300, 300);
}

std::shared_ptr<BackgroundImage> PhaseResourceManger::GetBackground(int idx) {
    return m_Background[idx];
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

void PhaseResourceManger::SetTime(int time) {
    this->time = time;
}

int PhaseResourceManger::GetTime() {
    return time;
}