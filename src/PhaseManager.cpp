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
        RESOURCE_DIR"/Temp/Image/gray.png",
        RESOURCE_DIR"/Blocks/Overworld/block.png",
        RESOURCE_DIR"/Scenery/Overworld/floorbricks.png",
        RESOURCE_DIR"/Blocks/Overworld/immovableBlock.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock0.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock1.png",
        RESOURCE_DIR"/Blocks/Overworld/misteryBlock2.png"
    };

    // 0: nothing 1: ground 2: blocks 3: immovable block 4: mistery block
    // 1 block size = 30

    for (int i = 0; i < imageFiles.size(); i++) {
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[i]->ChangeImg(imageFiles[i]);
        if (i != 0) {
            m_Background[i]->SetSize(2.0f, 2.0f);
        }else {
            m_Background[i]->SetSize(0.5f, 0.5f);
        }
        m_Background[i]->SetPosition(i * 30, -280);
    }
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
