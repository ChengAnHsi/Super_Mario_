#include "PhaseResourceManger.hpp"

#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger(std::shared_ptr<Mario> mario)
    : m_Mario(mario)  {
    m_MarioText = std::make_shared<TaskText>();
    m_MarioText->SetPosition(-400, 300);
    m_MarioText->SetTxtIdx(1, 0);

    m_MoneyText = std::make_shared<TaskText>();
    m_MoneyText->SetPosition(-130, 285);
    m_MoneyText->SetTxtIdx(2, 0);

    m_WorldText = std::make_shared<TaskText>();
    m_WorldText->SetPosition(200, 300);
    m_WorldText->SetTxtIdx(3, 1);

    m_TimeText = std::make_shared<TaskText>();
    m_TimeText->SetPosition(400, 300);
    m_TimeText->SetTxtIdx(4, 100);

    m_OtherText = std::make_shared<TaskText>();
    m_OtherText->SetTxtIdx(0, 0);

    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Scenery/Overworld/mountain2.png",
        RESOURCE_DIR"/Scenery/Overworld/bush1.png",
        RESOURCE_DIR"/Scenery/Overworld/floorbricks.png",
        RESOURCE_DIR"/Scenery/Overworld/floorbricks.png"
    };

    for (int i = 0; i < imageFiles.size(); i++) {
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[i]->ChangeImg(imageFiles[i]);
    }
    m_Background[0]->SetPosition(-400, -280);
    m_Background[1]->SetPosition(100, -280);
    m_Background[2]->SetPosition(-400, -300);
    m_Background[3]->SetPosition(100, -300);
}

void PhaseResourceManger::NextPhase() {
    if (m_Phase == 3) return;
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);
    m_Background[0]->NextPhase(m_Phase);
    /**test coin
    m_Mario->Increase_Coin(10);
    m_MoneyText->SetTxtIdx(2, m_Mario->Get_Coin());**/
    m_WorldText->SetTxtIdx(3, m_Phase++);
    m_OtherText->SetTxtIdx(5, 0);
}
