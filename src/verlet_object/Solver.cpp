/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Solver
*/

#include "Solver.hpp"
#include "MathVec2.hpp"

static sf::Vector2f gravity = {0.0f, 1000.f};

vle::Solver::Solver(std::vector<std::unique_ptr<VerletObject>>& objects,
    std::vector<std::unique_ptr<Link>>& links, unsigned int rate)
: m_objects(objects), m_links(links), m_grid({1920, 1080}, GLOB_OBJ_RADIUS)
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
        updatePartionning();
        applyGravity();
        solveCollisions();
        applyConstraint();
        updatePosition(subDt);
    }
}

void vle::Solver::updatePartionning()
{
    m_grid.clear();

    for (auto& it : m_objects) {
        m_grid.insert(it.get());
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


    for (int x{1}; x < m_grid.getDimensions().x - 1; ++x) {
        for (int y{1}; y < m_grid.getDimensions().y - 1; ++y) {
            auto& currentCell = m_grid.get({x, y});

            for (int dx{-1}; dx <= 1; ++dx) {
                for (int dy{-1}; dy <= 1; ++dy) {
                    auto& otherCell = m_grid.get({x + dx, y + dy});
                    checkCellsCollisions(currentCell, otherCell);
                }
            }
        }
    }
}

void vle::Solver::checkCellsCollisions(vle::Grid::Cell& cell1, vle::Grid::Cell& cell2)
{
    for (auto& objIdx1 : cell1.objects) {
        for (auto& objIdx2 : cell2.objects) {
            if (objIdx1 != objIdx2) {
                if (objIdx1->collide(*objIdx2))
                    solveCollision(*objIdx1, *objIdx2);
            }
        }
    }
}

void vle::Solver::solveCollision(VerletObject& obj1, VerletObject& obj2)
{
    float response_coef = 0.75f;

    sf::Vector2f v = obj1.getPosition() - obj2.getPosition();
    float dist2 = v.x * v.x + v.y * v.y;
    float min_dist = obj1.getRadius() + obj2.getRadius();
    // Check overlapping
    if (dist2 < min_dist * min_dist) {
        float dist = sqrt(dist2);
        sf::Vector2f n = v / dist;
        float mass_ratio_1 = obj1.getRadius() / (obj1.getRadius() + obj2.getRadius());
        float mass_ratio_2 = obj2.getRadius() / (obj1.getRadius() + obj2.getRadius());
        float delta = 0.5f * response_coef * (dist - min_dist);
        // Update positions
        obj1.move(-(n * (mass_ratio_2 * delta)));
        obj2.move((n * (mass_ratio_1 * delta)));
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
