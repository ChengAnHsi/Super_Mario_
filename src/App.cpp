#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");
    std::vector<std::string> marioImages = {RESOURCE_DIR"/Entities/mario_stand.png"};
    m_Mario = std::make_shared<Mario>(0,3,0,marioImages);
    m_Mario->SetPosition({-112.5f, 0.0f});
    m_Mario->SetZIndex(50);
    m_Mario->SetPlaying(true);
    m_Mario->SetLooping(true);
    m_Mario->m_Transform.scale = glm::vec2(2.0f, 2.0f);

    m_Root.AddChild(m_Mario);

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

    /**for (int i = 0; i < 3; ++i) {
        m_Doors.push_back(std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png"));
        m_Doors[i]->SetZIndex(5);
        m_Doors[i]->SetPosition({205.5f - 80.f * i, -3.5f});
        m_Doors[i]->SetVisible(false);
        m_Root.AddChild(m_Doors[i]);
    }**/

    m_PRM = std::make_shared<PhaseResourceManger>(m_Mario);
    m_Root.AddChildren(m_PRM->GetChildren());

    m_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/Sound/Music/Overworld/theme.mp3");
    m_BGM->SetVolume(70);
    m_BGM->Play();

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    float dis = m_Mario->move();

    if(m_Phase == Phase::Start) {
        m_Coin->SetLooping(true);
        m_Coin->SetPlaying(true);
    }
    else {
        m_Coin->SetLooping(false);
        m_Coin->SetPlaying(false);
    }
    
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

    m_Root.Update({dis,0.0f});
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
