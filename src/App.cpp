#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Global.hpp"

void App::Start() {
    LOG_TRACE("Start");
    std::vector<std::string> marioImages = {RESOURCE_DIR"/Entities/mario_stand.png"};
    m_Mario = std::make_shared<Mario>(0,3,0,marioImages);
    m_Mario->SetPosition({-360.0f + 2.5f * BLOCK_SIZE, -270.0f});
    m_Mario->SetZIndex(50);
    m_Mario->SetPlaying(true);
    m_Mario->SetLooping(true);
    m_Mario->m_Transform.scale = glm::vec2(2.0f, 2.0f);

    m_Root.AddChild(m_Mario);
    m_Camera = std::make_shared<Camera>();

    std::vector<std::string> coinImages;
    coinImages.reserve(3);
    for (int i = 0; i < 3; ++i) {
        coinImages.emplace_back(RESOURCE_DIR"/Collectibles/Underworld/ground_coin" + std::to_string(i) + ".png");
    }
    m_Coin = std::make_shared<AnimatedCharacter>(coinImages);
    m_Coin->SetPosition({-135.f, 285.f});
    m_Coin->SetZIndex(5);
    m_Coin->m_Transform.scale = glm::vec2(2.0f, 2.0f);
    m_Root.AddChild(m_Coin);

    m_PRM = std::make_shared<PhaseResourceManger>();

    m_Root.AddChildren(m_PRM->GetChildren());

    m_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
    m_BGM->SetVolume(70);
    m_BGM->Play();

    m_BM = std::make_shared<BlockManger>();
    m_Root.AddChildren(m_BM->GetChildren());
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if(m_Phase != Phase::Start) {
        m_Coin->SetLooping(true);
        m_Coin->SetPlaying(true);
        // fixed position
        m_Coin->SetPosition({-135.0f, 285.0f});
        m_PRM->ResetPosition();
        // decrease time after start game
        m_PRM->DecreaseTime();
    }

    if(m_PRM->GetTime() == 0) {
        m_Mario->Set_Live(m_Mario->Get_Live() - 1);
        if (m_Mario->Get_Live() == 0) {
            // dead situation
        }else {
            m_Mario->SetPosition({-360.0f + 2.5f * BLOCK_SIZE, -270.0f});
            m_PRM->SetTime(LEVEL_TIME[int(m_Phase) - 1]);
        }
    }

    // Calculate how far the camera should move to the right
    float dis = 0.0f;
    if(m_Phase != Phase::Start) {
        dis = m_Mario->move();
    }

    // Camera cannot move left
    if (dis < 0.0f) {
        dis = 0.0f;
    }
    // If Mario's position is less than -112.5, the camera does not need to move to the right
    if(m_Mario->GetPosition().x <= -112.5f) {
        dis = 0.0f;
    }

    // to solve mario left margin
    if(m_Mario->GetPosition().x < -360) {
        // Correct offset
        m_Mario->SetPosition({-360, m_Mario->GetPosition().y});
    }
    camera_movement_dis += dis;

    m_Root.Update({dis,0.0f});

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_EnterDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            ValidTask();
        }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
