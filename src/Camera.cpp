#include "Camera.hpp"
//
// Created by loedd on 2025/3/7.
//
void Camera::Update(std::shared_ptr<Mario> m_Mario) {
    m_Position = m_Mario->GetPosition();
}

float Camera::getPositionX() {
    return m_Position.x;
}