//
// Created by 112590007 on 2025/3/14.
//

#ifndef BLOCKMANAGER_HPP
#define BLOCKMANAGER_HPP

#include "BackgroundImage.hpp"
class BlockManger {
public:
    BlockManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (int i = 0; i < m_Backgrounds.size(); i++) {
            all_obj.emplace_back(m_Backgrounds[i]);
        }
        return all_obj;
    }

    void block_visible(float camera_movement_dis);
private:
    std::vector<std::shared_ptr<BackgroundImage>> m_Backgrounds;
    std::vector<float> m_PositionX;
    std::vector<float> m_PositionY;
    std::vector<bool> is_visible = {};
};

#endif //BLOCKMANAGER_HPP
