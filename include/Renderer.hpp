//
// Created by 112590007 on 2025/3/28.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include <vector>

#include "Util/GameObject.hpp"

class App;

/**
 * @class Renderer
 * @brief A class handling GameObjects' Draw()
 * @see Util::GameObject
 */
class Renderer final {
public:
    /**
     * @brief Parameterized constructor.
     *`
     *
     * @param children The GameObject needing to be managed by Renderer.
     */
    Renderer(const std::vector<std::shared_ptr<Util::GameObject>> &children = {});

    /**
     * @brief Add a child to Renderer.
     *
     * @param child The GameObject needing to be managed by Renderer.
     */
    void AddChild(const std::shared_ptr<Util::GameObject> &child);

    /**
     * @brief Add children to Renderer.
     *
     * @param children The GameObjects needing to be managed by Renderer.
     */
    void AddChildren(const std::vector<std::shared_ptr<Util::GameObject>> &children);

    /**
     * @brief Remove the child.
     *
     * @param child The GameObject being removed.
     */
    void RemoveChild(std::shared_ptr<Util::GameObject> child);

    /**
     * @brief Draw children according to their z-index.
     *
     * @note The user is not recommended to modify this function.
     */
    void Update(const glm::vec2 &offset = {0.0f, 0.0f});

private:
    std::vector<std::shared_ptr<Util::GameObject>> m_Children;
};
// namespace Util

#endif
