/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Solver
*/

#include "Solver.hpp"
#include "MathVec2.hpp"
#include <thread>

static sf::Vector2f gravity = {0.0f, 1000.f};

vle::Solver::Solver(std::vector<std::unique_ptr<VerletObject>>& objects,
    std::vector<std::unique_ptr<Link>>& links, unsigned int rate)
: m_objects(objects), m_links(links)
{
    setSimulationUpdateRate(rate);
}


void vle::Solver::setSimulationUpdateRate(unsigned int rate)
{
    m_frameDt = 1.0f / float(rate);
}

float vle::Solver::getTime() const
{
    return m_time;
}

float vle::Solver::getStepDt() const
{
    return m_frameDt / float(m_substeps);
}

void vle::Solver::update()
{
    m_time += m_frameDt;

    float subDt = getStepDt();
    for (size_t i(m_substeps); i--;) {
        updateTree();
        applyGravity();
        solveCollisions();
        applyConstraint();
        updatePosition(subDt);
    }
}

void vle::Solver::updateTree()
{
    m_tree.clear();
    for (auto& it : m_objects) {
        m_tree.insert(it.get());
    }
}

void vle::Solver::updatePosition(float dt)
{
    for (auto& it : m_objects) {
        it->updatePosition(dt);
    }
}

void vle::Solver::applyGravity()
{
    for (auto& it : m_objects) {
        it->accelerate(gravity);
    }
}

void vle::Solver::solveCollisions()
{
    size_t objects_count = m_objects.size();
    float response_coef = 0.75f;

    checks = 0;

    std::thread t1(&vle::Solver::solveCollisionsThread, this, 0, objects_count * 0.1);
    std::thread t2(&vle::Solver::solveCollisionsThread, this, objects_count * 0.1, objects_count * 0.2);
    std::thread t3(&vle::Solver::solveCollisionsThread, this, objects_count * 0.2, objects_count * 0.3);
    std::thread t4(&vle::Solver::solveCollisionsThread, this, objects_count * 0.3, objects_count * 0.4);
    std::thread t5(&vle::Solver::solveCollisionsThread, this, objects_count * 0.4, objects_count * 0.5);
    std::thread t6(&vle::Solver::solveCollisionsThread, this, objects_count * 0.5, objects_count * 0.6);
    std::thread t7(&vle::Solver::solveCollisionsThread, this, objects_count * 0.6, objects_count * 0.7);
    std::thread t8(&vle::Solver::solveCollisionsThread, this, objects_count * 0.7, objects_count * 0.8);
    std::thread t9(&vle::Solver::solveCollisionsThread, this, objects_count * 0.8, objects_count * 0.9);

    // Iterate on all objects
    for (size_t i{size_t(objects_count * 0.9)}; i < objects_count; ++i) {
        VerletObject &object_1 = *m_objects.at(i);
        // Iterate on object involved in new collision pairs
        std::vector<VerletObject*> found = m_tree.searchInRadius(object_1.getPosition(), object_1.getRadius() * 2);
        for (auto& object_2 : found) {
            if (&object_1 == object_2)
                continue;
            sf::Vector2f v = object_1.getPosition() - object_2->getPosition();
            float dist2 = v.x * v.x + v.y * v.y;
            float min_dist = object_1.getRadius() + object_2->getRadius();
            checks++;
            // Check overlapping
            if (dist2 < min_dist * min_dist) {
                float dist = sqrt(dist2);
                sf::Vector2f n = v / dist;
                float mass_ratio_1 = object_1.getRadius() / (object_1.getRadius() + object_2->getRadius());
                float mass_ratio_2 = object_2->getRadius() / (object_1.getRadius() + object_2->getRadius());
                float delta = 0.5f * response_coef * (dist - min_dist);
                // Update positions
                object_1.move(-(n * (mass_ratio_2 * delta)));
                object_2->move((n * (mass_ratio_1 * delta)));
            }
        }
    }

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();

    //std::cout << "checks this cicle: " << checks << std::endl;
}

void vle::Solver::solveCollisionsThread(unsigned int start, unsigned int end)
{
    float response_coef = 0.75f;
    // Iterate on all objects
    for (size_t i{start}; i < end; ++i) {
        VerletObject &object_1 = *m_objects.at(i);
        // Iterate on object involved in new collision pairs
        std::vector<VerletObject*> found = m_tree.searchInRadius(object_1.getPosition(), object_1.getRadius() * 2);
        for (auto& object_2 : found) {
            if (&object_1 == object_2)
                continue;
            sf::Vector2f v = object_1.getPosition() - object_2->getPosition();
            float dist2 = v.x * v.x + v.y * v.y;
            float min_dist = object_1.getRadius() + object_2->getRadius();
            checks++;
            // Check overlapping
            if (dist2 < min_dist * min_dist) {
                float dist = sqrt(dist2);
                sf::Vector2f n = v / dist;
                float mass_ratio_1 = object_1.getRadius() / (object_1.getRadius() + object_2->getRadius());
                float mass_ratio_2 = object_2->getRadius() / (object_1.getRadius() + object_2->getRadius());
                float delta = 0.5f * response_coef * (dist - min_dist);
                // Update positions
                object_1.move(-(n * (mass_ratio_2 * delta)));
                object_2->move((n * (mass_ratio_1 * delta)));
            }
        }
    }
}

void vle::Solver::applyConstraint()
{
    for (auto& link : m_links) {
        link->apply();
    }
    for (auto &obj : m_objects) {
        const sf::Vector2f v = m_constraintCenter - obj->getPosition();
        const float dist = mathVec2::length(v);
        if (dist > (m_constraintRadius - obj->getRadius())) {
            const sf::Vector2f n = v / dist;
            obj->setPosition(m_constraintCenter - n * (m_constraintRadius - obj->getRadius()));
        }
    }
}

float vle::Solver::getConstraintRadius()
{
    return m_constraintRadius;
}

sf::Vector2f vle::Solver::getConstraintPosition()
{
    return m_constraintCenter;
}
