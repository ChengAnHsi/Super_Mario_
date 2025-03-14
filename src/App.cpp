#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    std::vector<std::string> marioImages = {RESOURCE_DIR"/Entities/mario_stand.png"};
    m_Mario = std::make_shared<Mario>(0,3,0,marioImages);
    m_Mario->SetPosition({-112.5f, -270.0f});
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
    m_Coin->SetPosition({-180.f, 285.f});
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
    float dis = 0.0f;
    if(m_Phase != Phase::Start) {
        dis = m_Mario->move();
    }

    if(m_Phase != Phase::Start) {
        m_Coin->SetLooping(true);
        m_Coin->SetPlaying(true);
    }
    m_Coin->SetPosition({-180.0f, 280.0f});
    m_PRM->reset_position();

    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    if (m_EnterDown) {
        if (!Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
            ValidTask();
        }
    }
    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);

    // The camera cannot be moved to the left
    if (dis < 0.0f) {
        dis = 0.0f;
    }
    if(m_Mario->GetPosition().x <= -112.5f) {
        dis = 0.0f;
    }

    // to solve mario left margin
    if(m_Mario->GetPosition().x < -620) {
        // Correct offset
        m_Mario->SetPosition({-600, m_Mario->GetPosition().y});
    }


    // m_Camera->Update(m_Mario);

    // visible object if the camera captures the object
    camera_movement_dis += dis;

    // test

    /**if (camera_movement_dis == 22 * 30) {
        std::shared_ptr<BackgroundImage> tmp = m_PRM->get_Background(1);
        tmp->SetVisible(true);
        tmp->SetPosition(0.0f,0.0f);
    }
    if (camera_movement_dis >= 22*30 + 600) {
        std::shared_ptr<BackgroundImage> tmp = m_PRM->get_Background(1);
        tmp->SetVisible(false);
    }**/

    m_BM->block_visible(camera_movement_dis);
    //camera_movement_dis -= m_Camera->getPositionX();

    m_Root.Update({dis,0.0f});
    //m_Root.Update({dis,0.0f});

}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
