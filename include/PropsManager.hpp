//
// Created by 112590007 on 2025/3/14.
//

#ifndef BLOCKMANAGER_HPP
#define BLOCKMANAGER_HPP

#include "Props.hpp"
#include "Util/Renderer.hpp"

class PropsManager {
public:
    PropsManager();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (int i = 0; i < m_props.size(); i++) {
            all_obj.emplace_back(m_props[i]);
        }
        return all_obj;
    }
    std::vector<std::string> imagePaths = {

    };
    std::vector<int> GetX(int phase);
    std::vector<int> GetY(int phase);
    std::vector<int> Getidx(int phase);
    void SetProps(std::vector<std::shared_ptr<Props>> props);
    std::vector<std::shared_ptr<Props>> GetProps();
    std::vector<float> GetPosX();

private:
    // 0-1: block, 2-3: floor, ...
    // map 1-1
    std::vector<int> tmp_x = {};
    std::vector<int> tmp_y = {};
    std::vector<int> imgidx = {};
    //map 1-2
    std::vector<int> tmp_x2 = {};
    std::vector<int> tmp_y2 = {};
    std::vector<int> imgidx2 = {};
    //map 1-3
    std::vector<int> tmp_x3 = {};
    std::vector<int> tmp_y3 = {};
    std::vector<int> imgidx3 = {};
    std::vector<std::shared_ptr<Props>> m_props;
    std::vector<float> m_PositionX;
    std::vector<float> m_PositionY;
};

#endif //BLOCKMANAGER_HPP
