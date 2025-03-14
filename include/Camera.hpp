//
// Created by loedd on 2025/3/7.
//
#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Mario.hpp"
#include "Util/Transform.hpp"
class Camera{
    public:
        void Update( std::shared_ptr<Mario> m_Mario);
        float getPositionX();
private:
    glm::vec2 m_Position ={0.0f, 0.0f};
};


#endif //CAMERA_HPP
