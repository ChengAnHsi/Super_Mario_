#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Global.hpp"
#include "EnemyManager.hpp"

void App::Start() {
    LOG_TRACE("Start");
    std::vector<std::string> marioImages = {RESOURCE_DIR"/Entities/mario_stand.png"};
    m_Mario = std::make_shared<Mario>(0,3,0,marioImages);
    m_Mario->SetPosition({-380.0f + 2.5f * BLOCK_SIZE, -232.0f});
    m_Mario->SetZIndex(50);
    m_Mario->SetPlaying(true);
    m_Mario->SetLooping(true);
    m_Mario->m_Transform.scale = glm::vec2(MARIO_MAGNIFICATION, MARIO_MAGNIFICATION);

    m_Root.AddChild(m_Mario);

    std::vector<std::string> coinImages;
    coinImages.reserve(3);
    for (int i = 0; i < 3; ++i) {
        coinImages.emplace_back(RESOURCE_DIR"/Collectibles/Underworld/ground_coin" + std::to_string(i) + ".png");
    }
    m_Coin = std::make_shared<AnimatedCharacter>(coinImages);
    m_Coin->SetImages(coinImages, 1000, 0);
    m_Coin->SetPosition({-135.f, 285.f});
    m_Coin->SetZIndex(5);
    m_Coin->m_Transform.scale = glm::vec2(MARIO_MAGNIFICATION, MARIO_MAGNIFICATION);
    m_Root.AddChild(m_Coin);

    m_PRM = std::make_shared<PhaseResourceManger>();

    m_Root.AddChildren(m_PRM->GetChildren(true));

    m_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
    m_BGM->SetVolume(70);
    m_BGM->Play();

    m_BM = std::make_shared<BlockManager>();
    m_Root.AddChildren(m_BM->GetChildren());

//    m_Mario->AddCollisionBoxes(m_BM->GetBackground());
    m_Mario->AddCollisionBlocks(m_BM->GetBlocks());
    //m_Mario->AddCollisionBox(m_PRM->GetTube());

    m_EM = std::make_shared<EnemyManager>();
    // m_EM->SetAllEnemyCollisionBlocks(m_BM->GetBlocks());
    // m_EM->SetAllEnemyCollisionBoxs(m_BM->GetBackground());
    m_Root.AddChildren(m_EM->GetChildren());

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    if(m_Phase != Phase::Start) {
        m_Coin->SetLooping(true);
        m_Coin->SetPlaying(true);
        // decrease time after start game
        m_PRM->DecreaseTime();
        // check mario is in enemy visiion
        m_EM->SetEnemyMoving();

    }

    if(m_PRM->GetTime() == 0) {
        m_Mario->SetLive(m_Mario->GetLive() - 1);
        if (m_Mario->GetLive() == 0) {
            // TODO dead situation
        }else {
            m_Mario->SetPosition({-380.0f + 2.5f * BLOCK_SIZE, -232.0f});
            m_PRM->SetTime(LEVEL_TIME[int(m_Phase) - 1]);
        }
    }

    // Calculate how far the camera should move to the right
    float dis = 0.0f;
    if(m_Phase != Phase::Start) {
        dis = m_Mario->Move();
        // when mario move show coins he got
        m_PRM->SetCoin(m_Mario->GetCoin());
        m_BM->UpdatePropsAnimation();

        // Add this line to check for collisions with enemies
        m_EM->CheckMarioCollisions(m_Mario);
    }

    // Camera cannot move left
    if (dis < 0.0f) {
        dis = 0.0f;
    }

    // If Mario's position is less than this pos(<= -112.5), the camera does not need to move to the right
    if(m_Mario->GetPosition().x <= -112.5f) dis = 0.0f;

    // to solve mario left margin
    if(m_Mario->GetPosition().x < -360) {
        // Correct offset
        m_Mario->SetPosition({-360, m_Mario->GetPosition().y});
    }

    // lower than ground
    if(m_Mario->GetPosition().y < -360) {
        m_Mario->SetLive(m_Mario->GetLive() - 1);
        m_Mario->SetPosition({-380.0f + 2.5f * BLOCK_SIZE, 300.0f});
    }

    camera_movement_dis += dis;

    // fixed position
    m_PRM->ResetPosition(dis);
    m_Coin->m_Transform.translation.x += dis;

    if (Util::Input::IsKeyDown(Util::Keycode::A)) {
        m_Mario->SetPosition({-20.0f, 0.0f});
    }

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
