#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

#include "Global.hpp"
#include "App.hpp"

#include <iostream>

#include "Manager/EnemyManager.hpp"

void App::Start() {
    LOG_TRACE("Start");
    std::vector<std::string> marioImages = {RESOURCE_DIR"/Entities/mario_stand.png"};
    m_Mario = std::make_shared<Mario>(marioImages);
    m_Mario->SetPosition(2.5f * BLOCK_SIZE + BACKGROUND_X_OFFSET, 2.0f * BLOCK_SIZE + BACKGROUND_Y_OFFSET - 2.0f);
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
    m_Coin->SetPosition(-150.f, 285.f);
    m_Coin->SetZIndex(100);
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

    m_FPM = std::make_shared<FlyPlatformManager>();
    m_FPM->SetCeilingHeight(360.0f);
    m_CurrentState = State::UPDATE;
}

void App::Update() {
    // Calculate how far the camera should move to the right
    float dis = 0.0f;

    if(m_Phase != Phase::Start) {
        // decrease time after start game
        if (!m_Mario->GetReadyNextPhase()) m_PRM->DecreaseTime();
        if(m_PRM->GetTime() == 0) {
            if (m_Mario->GetReadyNextPhase()) {
                NextPhase(true);
            }
            else m_Mario->Die();
        }

        // check mario is in enemy visiion
        m_EM->SetEnemyMoving();

        // check platform can move
        m_FPM->CheckMovingPlatform();

        dis = m_Mario->Move();

        // show scores
        m_PRM->SetScore(m_Mario->GetScore());

        // show coins
        m_PRM->SetCoin(m_Mario->GetCoin());

        // show live
        m_PRM->SetLive(m_Mario->GetLive());

        // update all activated props animation
        m_PM->UpdatePropsAnimation();

        // check props and mario collision
        m_PM->CheckCollisionWithMario(m_Mario);

        // update all activated blocks animation
        m_BM->UpdateBlocksAnimation();

        // check for collisions with enemies
        m_EM->CheckMarioCollisions(m_Mario);

        // platform moving
        m_FPM->UpdatePlatforms(1.0f);

        // update each fireball pos and collision
        m_FM->UpdateFireballInvalidState();
        m_FM->UpdateFireballsAnimation();
        auto fireball = m_FM->GetNewFireball();
        if (fireball) m_Root.AddChild(fireball);
        m_Mario->IncreaseScore(m_EM->CheckFireBallCollisionsAndGotPoint(m_FM->GetFireballs()));
    }

    // Camera cannot move left
    if (dis < 0.0f) dis = 0.0f;

    // If Mario's position is less than this pos(<= -112.5), the camera does not need to move to the right
    if(m_Mario->GetPosition().x <= -112.5f) dis = 0.0f;
    // camera need to update faster, when mario is standing on platform
    if (m_Mario->GetPosition().x >= -105.0f) dis += 2.0f;

    // to solve mario left margin
    if(m_Mario->GetPosition().x < -360) {
        // Correct offset
        m_Mario->SetPosition(-360, m_Mario->GetPosition().y);
    }

    // lower than ground
    if(m_Mario->GetPosition().y < -360 && !m_Mario->GetTimeToMoveCamera()) {
        m_Mario->SetDying(false);
        m_Mario->SetLive(m_Mario->GetLive() - 1);
        if (m_Mario->GetLive() > 0) {
            // reset phase
            NextPhase(false);
        }else {
            // game over
            m_Mario->SetDead(true);
        }
    }

    camera_movement_dis += dis;

    // fixed position
    m_PRM->ResetPosition(dis, 0);
    m_Coin->m_Transform.translation.x += dis;

    // move camera
    m_Root.Update({dis,0.0f});
    // if mario drill tube then move the camera
    if(m_Mario->GetTimeToMoveCamera()) {
        // update 1-2 background sky image(blue)
        m_Mario->SetPosition(m_Mario->GetPosition().x, m_Mario->GetPosition().y + 9 * BLOCK_SIZE);
        m_Mario->SetDrill(true);
        m_Mario->SetDrillState(DrillState::Up);
        m_Mario->SetDrillDistance(4 * BLOCK_SIZE);

        // fix pos
        float camera_move_dis = 167.5f * BLOCK_SIZE - camera_movement_dis;
        m_Root.Update({camera_move_dis,720.0f});
        m_PRM->ResetPosition(camera_move_dis, 720.0f);
        m_Coin->SetPosition(m_Coin->GetPosition().x + camera_move_dis, m_Coin->GetPosition().y + 720.0f);
        m_Mario->SetTimeToMoveCamera(false);
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_Mario->GetLive() == 0) {
        m_Phase = Phase::Level1_3;
        NextPhase(true);
    }

    m_PRM->CheckMarioCollisionFlag(m_Mario);
    if(!m_Mario->GetDrill() && m_Phase == Phase::Level1_2) m_PRM->CheckMarioCollisionTube(m_Mario);
    if (m_Mario->GetReadyNextPhase()) {
        m_PRM->ConvertTimeToScore(m_Mario);
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
