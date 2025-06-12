#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/SFX.hpp"
#include "Util/BGM.hpp"

#include "Renderer.hpp"
#include "AnimatedCharacter.hpp"
#include "Mario.hpp"
#include "Manager/FlyPlatformManager.hpp"
#include "Manager/PhaseResourceManger.hpp"
#include "Manager/BlockManager.hpp"
#include "Manager/EnemyManager.hpp"
#include "Manager/PropsManager.hpp"
#include "Manager/FireballManager.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    [[nodiscard]] State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void NextPhase(bool is_nextphase);
    void ResetPhase();

    enum class Phase {
        Start = 0,
        Level1_1,
        Level1_2,
        Level1_3
    };

    State m_CurrentState = State::START;
    Phase m_Phase = Phase::Start;

    Renderer m_Root;

    std::shared_ptr<Mario> m_Mario;

    std::shared_ptr<AnimatedCharacter> m_Coin;
    std::shared_ptr<PhaseResourceManger> m_PRM;
    std::shared_ptr<BlockManager> m_BM;
    std::shared_ptr<EnemyManager> m_EM;
    std::shared_ptr<PropsManager> m_PM;
    std::shared_ptr<FireballManager> m_FM;
    std::shared_ptr<Util::BGM> m_BGM;
    std::shared_ptr<Util::SFX> m_SFX;
    std::shared_ptr<FlyPlatformManager> m_FPM;
    float camera_movement_dis = 112.5f;
    bool m_EnterDown = false;
};

#endif
