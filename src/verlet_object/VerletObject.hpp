/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** VerletObject
*/

#ifndef VERLETOBJECT_HPP_
    #define VERLETOBJECT_HPP_

    #include "common.hpp"

    namespace vle {
        class VerletObject
        {
            public:
                VerletObject(float mass, sf::Vector2f pos, float radius, sf::Color color);
                ~VerletObject();
                void updatePosition(float deltaTime);
                void accelerate(sf::Vector2f acc);

                // Getters and setters for position, velocity, and mass
                const sf::Vector2f& getPosition() const;
                void setPosition(const sf::Vector2f& newPosition);

                bool collide(const VerletObject& other) const;

                void move(const sf::Vector2f& offset);

                const sf::Vector2f& getAcceleration() const;
                void setAcceleration(const sf::Vector2f& newVelocity);

                float getMass() const;
                void setMass(float newMass);

                void setRadius(float newRadius);
                float getRadius() const;

                void setFixed(bool fixed);
                bool isFixed() const;

                sf::Vector2f getLastPosition() const;

                sf::Vertex* getVertices() const;

            private:
                float m_mass;
                float m_radius;
                float m_shapeSize;
                bool m_fixed;
                sf::Color m_color;
                sf::Vector2f m_acceleration;
                sf::Vector2f m_position;
                sf::Vector2f m_lastPosition;
                sf::Vertex m_vertices[4];

            private:
                void updateVertices();
        };
    }
#endif /* !VERLETOBJECT_HPP_ */
