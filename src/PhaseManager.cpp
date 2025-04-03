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

    // background image setting
    // [0]: logo
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetPosition(0, 0);
    m_Background.back()->SetScale(3.0f,3.0f);
    // The background z-index is placed at the bottom
    m_Background.back()->SetZIndex(-50);
    // [1]: start mountain(left)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetImage(imageFiles[5]);
    m_Background.back()->SetPosition(-240.0f, 2.5 * BLOCK_SIZE - 315.0f);
    m_Background.back()->SetScale(1.5f,1.5f);
    // [2]: start bush(right)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetImage(imageFiles[0]);
    m_Background.back()->SetPosition(11 * BLOCK_SIZE - 360.0f, 2 * BLOCK_SIZE- 325.0f);
    m_Background.back()->SetScale(1.3f,1.3f);

    // TODO use function fix pos(not finish)
    // m_Tube.back()->SetPosition(28 * BLOCK_SIZE + BACKGROUND_X_OFFSET + m_Tube.back()->GetSize().x / 2, 2.5 * BLOCK_SIZE + BACKGROUND_Y_OFFSET - m_Tube.back()->GetSize().y / 2);

    // tube init(map 1-1)
    for (int i = 0; i < tmp_x.size(); i++) {
        m_Tube.push_back(std::make_shared<BackgroundImage>());
        m_Tube.back()->SetImage(imagePaths[imgidx[i]]);
        m_Tube.back()->SetPosition(tmp_x[i] * BLOCK_SIZE + x_offset[imgidx[i]], tmp_y[i] * BLOCK_SIZE  + y_offset[imgidx[i]]);
        m_Tube.back()->SetScale(tube_magnification[imgidx[i]], tube_magnification[imgidx[i]]);
    }
}

void PhaseResourceManger::NextPhase(int m_Phase, std::shared_ptr<Util::BGM> m_BGM) {
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);

    if (m_Phase == 1){
        // [0]: blue background image
        m_Background[0]->NextPhase(m_Phase);
        m_Background[0]->SetScale(80.0f,7.0f);
        m_Background[0]->SetZIndex(-50);
        // [1]: castle is visible
        // map 1-1 castle
        m_Background[1]->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(202 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE - 325.0f);
        m_Background[1]->SetScale(3.0f, 3.0f);
        // [2]: flag set
        m_Background[2]->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(198 * BLOCK_SIZE - 320.0f, 8 * BLOCK_SIZE - 390.0f);
        m_Background[2]->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        // TODO [3]> mountain, cloud... set invisible use app->nextphase
    }
    if (m_Phase == 2){
        m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Underworld/theme.mp3");
        m_BGM->Play();
        m_Background.clear();
        // [0]: black background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[0]->NextPhase(m_Phase);
        m_Background[0]->SetScale(80.0f,7.0f);
        m_Background[0]->SetZIndex(-50);
        // [1]: castle is visible
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[1]->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(202 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE - 325.0f);
        m_Background[1]->SetScale(3.0f, 3.0f);
        // [2]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[2]->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(198 * BLOCK_SIZE - 320.0f, 8 * BLOCK_SIZE - 390.0f);
        m_Background[2]->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);

        // tube init(map 1-2)
        m_Tube.clear();
        for (int i = 0; i < tmp_x2.size(); i++) {
            m_Tube.push_back(std::make_shared<BackgroundImage>());
            m_Tube.back()->SetImage(imagePaths[imgidx2[i]]);
            m_Tube.back()->SetPosition(tmp_x2[i] * BLOCK_SIZE + x_offset[imgidx2[i]], tmp_y2[i] * BLOCK_SIZE  + y_offset[imgidx2[i]]);
            m_Tube.back()->SetScale(tube_magnification[imgidx[i]], tube_magnification[imgidx2[i]]);
        }
        // top tube
        m_Tube.push_back(std::make_shared<BackgroundImage>());
        m_Tube.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-medium-tube.png");
        m_Tube.back()->SetPosition(2 * BLOCK_SIZE + x_offset[1], 14 * BLOCK_SIZE + y_offset[1]);
        m_Tube.back()->SetScale(tube_magnification[1], -tube_magnification[1]);
        // end horizontal tube
        m_Tube.push_back(std::make_shared<BackgroundImage>());
        m_Tube.back()->SetImage(RESOURCE_DIR"/Scenery/horizontal-final-tube.png");
        m_Tube.back()->SetPosition(167 * BLOCK_SIZE + x_offset[1], 6 * BLOCK_SIZE + y_offset[0]);
        m_Tube.back()->SetScale(tube_magnification[0], tube_magnification[0]);
        // end vertical tube
        m_Tube.push_back(std::make_shared<BackgroundImage>());
        m_Tube.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-xlarge-tube.png");
        m_Tube.back()->SetPosition(168 * BLOCK_SIZE + x_offset[0], 11 * BLOCK_SIZE + y_offset[2]);
        m_Tube.back()->SetScale(tube_magnification[0], tube_magnification[0]);
    }
    if (m_Phase == 3) {
        m_BGM->LoadMedia(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
        m_BGM->Play();
        m_Background.clear();
        // [0]: black background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[0]->NextPhase(m_Phase);
        m_Background[0]->SetScale(80.0f,7.0f);
        m_Background[0]->SetZIndex(-50);
        // [1]: castle is visible
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[1]->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(202 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE - 325.0f);
        m_Background[1]->SetScale(3.0f, 3.0f);
        // [2]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[2]->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(198 * BLOCK_SIZE - 320.0f, 8 * BLOCK_SIZE - 390.0f);
        m_Background[2]->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        // TODO append platform to background
        // TODO getter: platform, add them to mario's collision box
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

std::vector<std::shared_ptr<BackgroundImage> > PhaseResourceManger::GetTube() {
    return m_Tube;
}

std::vector<std::shared_ptr<BackgroundImage> > PhaseResourceManger::GetBackground() {
    return m_Background;
}