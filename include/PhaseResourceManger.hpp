#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "TaskText.hpp"
#include "Character.hpp"
#include "BackgroundImage.hpp"
#include "Mario.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj= {m_MarioText, m_MoneyText, m_WorldText, m_TimeText, m_OtherText};
        for (int i = 0; i < m_Background.size(); i++) {
            all_obj.emplace_back(m_Background[i]);
        }
        return all_obj;
    }

    void NextPhase();
    void reset_position() const;
    std::shared_ptr<BackgroundImage> get_Background(int idx);

private:
    std::shared_ptr<TaskText> m_MarioText;
    std::shared_ptr<TaskText> m_MoneyText;
    std::shared_ptr<TaskText> m_WorldText;
    std::shared_ptr<TaskText> m_TimeText;
    std::shared_ptr<TaskText> m_OtherText;
    std::vector<std::shared_ptr<BackgroundImage>> m_Background;
    std::shared_ptr<Mario> m_Mario;

    int m_Phase = 1;
    int time = 0;
};


#endif //PHASE_MANGER_HPP
