//
// Created by 112590007 on 2025/4/11.
//
#include "PropsManager.hpp"
#include "FireFlower.hpp"
#include "OneUpMushroom.hpp"
#include "Starman.hpp"
#include "MagicMushroom.hpp"
#include "BackgroundImage.hpp"
#include "Global.hpp"
#include "App.hpp"

#include <iostream>
PropsManager::PropsManager() {

     int imgidx_size = imgidx.size();
     for (int i = 0; i < imgidx_size; i++) {
         m_PositionX.push_back(tmp_x[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET);
         m_PositionY.push_back(tmp_y[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
         if(imgidx[i] == 0) {
             m_props.push_back(std::make_shared<OneUpMushroom>());
             m_props.back()->SetImage(imagePaths[imgidx[i]]);

         } else if(imgidx[i] == 1) {
             m_props.push_back(std::make_shared<MagicMushroom>());
             m_props.back()->SetImage(imagePaths[imgidx[i]]);
         }else if(imgidx[i] == 3) {
        m_props.push_back(std::make_shared<Starman>());
        m_props.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i]+1],imagePaths[imgidx[i]+2],imagePaths[imgidx[i]+3],imagePaths[imgidx[i]+4]},50,0);
    }else if(imgidx[i] == 8 || imgidx[i] == 12) {
        m_props.push_back(std::make_shared<FireFlower>());
        m_props.back()->SetImage({imagePaths[imgidx[i]],imagePaths[imgidx[i]+1],imagePaths[imgidx[i]+2],imagePaths[imgidx[i]+3]},50,0);
    }
    m_props.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
    m_props.back()->SetPosition(m_PositionX[i],m_PositionY[i]);
    }
}
std::vector<float> PropsManager::GetPosX() {
    return m_PositionX;
}

std::vector<int> PropsManager::GetX(int phase){
    std::cout << phase;
    switch (phase)
    {
        case 1:
            return tmp_x;
        case 2:
            return tmp_x2;
        case 3:
            return tmp_x3;
        default:
            return tmp_x;
    }
}

std::vector<int> PropsManager::GetY(int phase){
    switch (phase)
    {
        case 1:
            return tmp_y;
        case 2:
            return tmp_y2;
        case 3:
            return tmp_y3;
        default:
            return tmp_y;
    }
}

std::vector<int> PropsManager::Getidx(int phase){
    switch (phase)
    {
        case 1:
            return imgidx;
        case 2:
            return imgidx2;
        case 3:
            return imgidx3;
        default:
            return imgidx;
    }
}
void PropsManager::SetProps(std::vector<std::shared_ptr<Props>> props){
    this->m_props.clear();
    this->m_props = props;
}

std::vector<std::shared_ptr<BackgroundImage>> PropsManager::GetBackground() {
    std::vector<std::shared_ptr<BackgroundImage>> all_prop;
    for (int i = 0; i < m_props.size(); i++) {
        all_prop.push_back(std::static_pointer_cast<BackgroundImage>(m_props[i]));
    }
    return all_prop;
}


std::vector<std::shared_ptr<Props>> PropsManager::GetProps(){
    return m_props;
}