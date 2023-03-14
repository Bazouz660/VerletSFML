/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** VerletObject
*/

#include "VerletObject.hpp"
#include "ResourceManager.hpp"

namespace vle {

VerletObject::VerletObject(float m_mass, sf::Vector2f pos, float radius, sf::Color color)
: m_mass(m_mass), m_position(pos), m_radius(radius), m_color(color)
{
    m_fixed = false;
    m_lastPosition = pos;
    m_acceleration = {0, 0};
    m_shapeSize = m_radius * 2;

    sf::Texture* texture = vle::ResourceManager::getInstance().getTexture("circle");
    sf::Vector2u textureSize = texture->getSize();

    for (auto& vertex : m_vertices)
        vertex.color = m_color;
    m_vertices[0].texCoords = {0, 0};
    m_vertices[1].texCoords = {textureSize.x, 0};
    m_vertices[2].texCoords = {textureSize.x, textureSize.y};
    m_vertices[3].texCoords = {0, textureSize.y};

    m_vertices[0].position = m_position;
    m_vertices[1].position = {m_position.x + m_shapeSize, m_position.y};
    m_vertices[2].position = {m_position.x + m_shapeSize, m_position.y + m_shapeSize};
    m_vertices[3].position = {m_position.x, m_position.y + m_shapeSize};

    m_vertices[0].position -= {m_radius, m_radius};
    m_vertices[1].position -= {m_radius, m_radius};
    m_vertices[2].position -= {m_radius, m_radius};
    m_vertices[3].position -= {m_radius, m_radius};
}

VerletObject::~VerletObject()
{
}

bool VerletObject::collide(const VerletObject& other) const
{
    double squared_distance = pow(m_position.x - other.getPosition().x, 2) + pow(m_position.y - other.getPosition().y, 2);
    double squared_sum_of_radii = pow(getRadius() + other.getRadius(), 2);
    return squared_distance < squared_sum_of_radii;
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

    updateVertices();
}

void VerletObject::updateVertices()
{
    m_vertices[0].position = m_position;
    m_vertices[1].position = {m_position.x + m_shapeSize, m_position.y};
    m_vertices[2].position = {m_position.x + m_shapeSize, m_position.y + m_shapeSize};
    m_vertices[3].position = {m_position.x, m_position.y + m_shapeSize};

    m_vertices[0].position -= {m_radius, m_radius};
    m_vertices[1].position -= {m_radius, m_radius};
    m_vertices[2].position -= {m_radius, m_radius};
    m_vertices[3].position -= {m_radius, m_radius};
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

sf::Vertex* VerletObject::getVertices() const
{
    return (sf::Vertex*)m_vertices;
}

}