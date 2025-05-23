#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

#include "Global.hpp"
#include "App.hpp"

#include "Manager/EnemyManager.hpp"

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
    m_Coin->SetPlaying(false);
    m_Coin->m_Transform.scale = glm::vec2(MARIO_MAGNIFICATION, MARIO_MAGNIFICATION);
    m_Root.AddChild(m_Coin);

    m_PRM = std::make_shared<PhaseResourceManger>();

    m_Root.AddChildren(m_PRM->GetChildren(true));

    m_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
    m_BGM->SetVolume(70);
    m_BGM->Play();

    m_BM = std::make_shared<BlockManager>();
    m_Root.AddChildren(m_BM->GetChildren());

    m_PM = std::make_shared<PropsManager>();
    m_Root.AddChildren(m_PM->GetChildren());

    m_EM = std::make_shared<EnemyManager>();
    m_Root.AddChildren(m_EM->GetChildren());

    m_FM = std::make_shared<FireballManager>();
    m_Mario->SetFireballManager(m_FM);

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

    if(m_PRM->GetTime() == 0) m_Mario->Die();

    // Calculate how far the camera should move to the right
    float dis = 0.0f;
    if(m_Phase != Phase::Start) {
        dis = m_Mario->Move();

        // show scores
        m_PRM->SetScore(m_Mario->GetScore());

        // show coins
        m_PRM->SetCoin(m_Mario->GetCoin());

        // update all activated props animation
        m_PM->UpdatePropsAnimation();

        // check props and mario collision
        m_PM->CheckCollisionWithMario(m_Mario);

        // update all activated blocks animation
        m_BM->UpdateBlocksAnimation();

        // check for collisions with enemies
        m_EM->CheckMarioCollisions(m_Mario);

        // update each fireball pos and collision
        m_FM->UpdateFireballInvalidState();
        m_FM->UpdateFireballsAnimation();
        auto fireball = m_FM->GetNewFireball();
        if (fireball) {
            m_Root.AddChild(fireball);
        }
        m_Mario->IncreaseScore(m_EM->CheckFireBallCollisionsAndGotPoint(m_FM->GetFireballs()));
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
        m_Mario->is_dying = false;
        m_Mario->SetLive(m_Mario->GetLive() - 1);
        if (m_Mario->GetLive() > 0) {
            // reset phase
            NextPhase(false);
        }else {
            // game over
            m_Mario->is_dead = true;
        }
    }

    camera_movement_dis += dis;

    // fixed position
    m_PRM->ResetPosition(dis);
    m_Coin->m_Transform.translation.x += dis;

    // move camera
    m_Root.Update({dis,0.0f});

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_Mario->GetLive() == 0) {
        // TODO gameover
        m_CurrentState = State::END;
    }

    if (m_EnterDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            NextPhase(true);
        }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
