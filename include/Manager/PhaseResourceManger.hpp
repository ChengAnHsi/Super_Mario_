#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "TaskText.hpp"
#include "BackgroundImage.hpp"
#include "Mario.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren(const bool is_init) const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        if (is_init){
            all_obj = {m_ScoreText, m_MoneyText, m_WorldText, m_TimeText, m_OtherText};
        }
        for (const auto & img : m_Background) {
            all_obj.emplace_back(img);
        }
        for (const auto & img : m_CollisionBoxes) {
            all_obj.emplace_back(img);
        }
        for (const auto & img : m_CollectibleCoins) {
            all_obj.emplace_back(img);
        }
        return all_obj;
    }
    std::vector<std::string> imagePaths = {
        RESOURCE_DIR"/Scenery/vertical-small-tube.png",
        RESOURCE_DIR"/Scenery/vertical-medium-tube.png",
        RESOURCE_DIR"/Scenery/vertical-large-tube.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin0.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin1.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin2.png", // 5
        RESOURCE_DIR"/Collectibles/Overworld/coin3.png",
        RESOURCE_DIR"/Collectibles/Underworld/ground_coin0.png",
        RESOURCE_DIR"/Collectibles/Underworld/ground_coin1.png",
        RESOURCE_DIR"/Collectibles/Underworld/ground_coin2.png"
    };

    void NextPhase(int m_phase);
    bool CheckMarioCollisionFlag(std::shared_ptr<Mario> mario, int m_phase);
    // 1-2
    bool CheckMarioCollisionTube(std::shared_ptr<Mario> mario, int m_phase);
    // fixed texts
    void ResetPosition(float dis) const;

    void DecreaseTime();
    void ConvertTimeToScore(std::shared_ptr<Mario> mario);

    // setter and getter
    void SetTime(int time);
    int GetTime() const;
    std::vector<std::shared_ptr<BackgroundImage>> GetBackground();
    std::vector<std::shared_ptr<BackgroundImage>> GetCollisionBoxes();
    void SetCoin(int coin) const;
    std::vector<std::shared_ptr<BackgroundImage>> GetCollectibleCoins();
    void SetScore(int score) const;

private:
    std::shared_ptr<TaskText> m_ScoreText;
    std::shared_ptr<TaskText> m_MoneyText;
    std::shared_ptr<TaskText> m_WorldText;
    std::shared_ptr<TaskText> m_TimeText;
    std::shared_ptr<TaskText> m_OtherText;
    std::vector<std::shared_ptr<BackgroundImage>> m_Background;
    std::vector<std::shared_ptr<BackgroundImage>> m_CollisionBoxes;
    std::vector<std::shared_ptr<BackgroundImage>> m_CollectibleCoins;

    int time = 400;
    std::chrono::steady_clock::time_point last_update;

    // image index 0-2: tube
    // tube & lifting platform position
    std::vector<float> tubex_offset = {-310, -313, -313};
    std::vector<float> tubey_offset = {-350, -375, -400};
    std::vector<float> tube_magnification = {3.0f, 3.0f, 3.0f};
    // map 1-1 tube
    std::vector<int> collisionboxes_x = {28, 38, 46, 57, 163, 179};
    std::vector<int> collisionboxes_y = {3, 4, 5, 5, 3, 3};
    std::vector<int> collisionboxes_imgidx = {0, 1, 2, 2, 0, 0};

    // map 1-2 tube
    // TODO add 1-2 Lifting platform
    std::vector<int> collisionboxes_x2 = {103, 109, 115};
    std::vector<int> collisionboxes_y2 = {4, 5, 3};
    std::vector<int> collisionboxes_imgidx2 = {2, 2, 2};

    // map 1-3 no tube here
    // TODO add 1-3 Lifting platform
    // std::vector<int> collisionboxes_x3 = {};
    // std::vector<int> collisionboxes_y3 = {};
    // std::vector<int> collisionboxes_imgidx3 = {};

    // image index coin(overworld): 3-6, coin(underworld): 7-9
    // coin position
    // map 1-2
    std::vector<int> coin_x2 = {41,42,43,44,84,85,86,87,88,89,
        40,45,58,59,60,61,68};
    std::vector<int> coin_y2 = {9,9,9,9,9,9,9,9,9,9,
        6,6,6,6,6,6,6};
    std::vector<int> coin_imgidx2 = {7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7};
    // map 1-3
    std::vector<int> coin_x3 = {37,38,
        27,28,29,60,61,62,63,93,94,97,98,
        85,86,120,121,
        50,51,
        113,114,115};
    std::vector<int> coin_y3 = {12,12,
        10,10,10,10,10,10,10,10,10,10,10,
        9,9,9,9,
        8,8,
        2,2,2};
    std::vector<int> coin_imgidx3 = {3,3,
        3,3,3,3,3,3,3,3,3,3,3,
        3,3,3,3,
        3,3,
        3,3,3};
};


#endif //PHASE_MANGER_HPP
