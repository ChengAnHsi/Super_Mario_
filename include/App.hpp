#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Renderer.hpp"
#include "Util/BGM.hpp"
#include "PhaseResourceManger.hpp"
#include "BlockManager.hpp"
#include "AnimatedCharacter.hpp"
#include "Mario.hpp"
#include "EnemyManager.hpp"
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();
    void NextPhase();

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
    std::shared_ptr<Util::BGM> m_BGM;

    float camera_movement_dis = 0;//-380.0f + 2.5f * BLOCK_SIZE;
    bool m_EnterDown = false;
};

#endif
