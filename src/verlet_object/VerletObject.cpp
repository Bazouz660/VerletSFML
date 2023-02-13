/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** VerletObject
*/

#include "VerletObject.hpp"

namespace vle {

VerletObject::VerletObject(float m_mass, sf::Vector2f pos, float radius, sf::Color color)
: m_mass(m_mass), m_position(pos), m_radius(radius), m_color(color)
{
    m_fixed = false;
    m_lastPosition = pos;
    m_position = pos;
    m_acceleration = {0, 0};
    m_shape.setRadius(radius);
    m_shape.setFillColor(color);
    m_shape.setPosition(pos);
    m_shape.setOrigin(m_radius, m_radius);
}

VerletObject::~VerletObject()
{
}

void VerletObject::updatePosition(float dt)
{
    if (isFixed())
        return;
    // Compute how much we moved
    const sf::Vector2f displacement = m_position - m_lastPosition;
    // Update position
    m_lastPosition = m_position;
    m_position = m_position + displacement + m_acceleration * (dt * dt);
    // Reset acceleration
    m_acceleration = {};
}

void VerletObject::setFixed(bool fixed)
{
    m_fixed = fixed;
}

bool VerletObject::isFixed() const
{
    return m_fixed;
}

void VerletObject::accelerate(sf::Vector2f acc)
{
    m_acceleration += acc;
}

sf::Vector2f VerletObject::getLastPosition() const
{
    return m_lastPosition;
}

const sf::Vector2f& VerletObject::getPosition() const
{
    return m_position;
}

void VerletObject::move(const sf::Vector2f& offset)
{
    if (!isFixed())
        m_position += offset;
}

void VerletObject::setPosition(const sf::Vector2f& newPosition)
{
    m_position = newPosition;
}

const sf::Vector2f& VerletObject::getAcceleration() const
{
    return m_acceleration;
}

void VerletObject::setAcceleration(const sf::Vector2f& newVelocity)
{
    m_acceleration = newVelocity;
}

float VerletObject::getMass() const
{
    return m_mass;
}

void VerletObject::setMass(float newMass)
{
    m_mass = newMass;
}

void VerletObject::setRadius(float newRadius)
{
    m_radius = newRadius;
}

float VerletObject::getRadius() const
{
    return m_radius;
}

void VerletObject::draw(sf::RenderTarget& target)
{
    m_shape.setPosition(m_position);

    target.draw(m_shape);
}

}