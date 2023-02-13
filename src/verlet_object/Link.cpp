/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Link
*/

#include "Link.hpp"
#include "MathVec2.hpp"

vle::Link::Link(VerletObject& obj1, VerletObject& obj2, float targetDist)
: m_obj1(obj1), m_obj2(obj2), m_targetDist(targetDist)
{
}

vle::Link::~Link()
{
}

void vle::Link::apply()
{
    sf::Vector2f axis = m_obj1.getPosition() - m_obj2.getPosition();
    float dist = mathVec2::length(axis);
    sf::Vector2f n = axis / dist;
    float delta = m_targetDist - dist;
    m_obj1.move((0.5f * delta * n));
    m_obj2.move(-(0.5f * delta * n));
}
