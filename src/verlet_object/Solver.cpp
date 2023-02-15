/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Solver
*/

#include "Solver.hpp"
#include "MathVec2.hpp"
#include <functional>
#include <thread>
#include <execution>

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

    auto [width, height] = m_grid.getDimensions();

    std::vector<std::unique_ptr<std::thread>> threads;
    int numThreads = std::thread::hardware_concurrency();
    int start = 0;
    int end = width / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::unique_ptr<std::thread>(new std::thread (&vle::Solver::threadFunction, this, start, end)));
        start = end;
        end += width / numThreads;
    }

    for (auto& thread : threads) {
        thread->join();
    }

    return;

    for (auto& row : m_grid.getCells()) {
        for (auto& currentCell : *row) {
            auto [x, y] = currentCell->position;
            if (x > 0 && y > 0 && x < width - 1 && y < height - 1) {
                for (int dx{-1}; dx <= 1; ++dx) {
                    for (int dy{-1}; dy <= 1; ++dy) {
                        auto& otherCell = m_grid.get({x + dx, y + dy});
                        checkCellsCollisions(*currentCell, otherCell);
                    }
                }
            }
        }
    }
}

void vle::Solver::threadFunction(int startRow, int endRow) {
    auto [width, height] = m_grid.getDimensions();
    std::for_each(m_grid.getCells().begin() + startRow, m_grid.getCells().begin() + endRow, [this, width, height](const auto& row) {
        for (auto& col : *row) {
            auto [x, y] = col->position;
            if (x > 0 && y > 0 && x < width - 1 && y < height - 1) {
                for (int dx{-1}; dx <= 1; ++dx) {
                    for (int dy{-1}; dy <= 1; ++dy) {
                        auto& otherCell = m_grid.get({x + dx, y + dy});
                        checkCellsCollisions(*col, otherCell);
                    }
                }
            }
        }
    });
}


void vle::Solver::drawGrid(sf::RenderTarget& target)
{
    for (auto& rows : m_grid.getCells()) {
        for (auto& cols : *rows.get()) {
            sf::RectangleShape shape;
            shape.setPosition(sf::Vector2f(cols->position * m_grid.getCellSize()));
            shape.setSize({(float)m_grid.getCellSize(), (float)m_grid.getCellSize()});
            shape.setOrigin(shape.getSize().x * 0.5, shape.getSize().y * 0.5);
            shape.move(shape.getSize().x * 0.5, shape.getSize().y * 0.5);
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::Blue);
            shape.setOutlineThickness(1);

            target.draw(shape);
        }
    }
}

void vle::Solver::checkCellsCollisions(vle::Grid::Cell& cell1, vle::Grid::Cell& cell2)
{
    for (auto& objIdx1 : cell1.objects) {
        for (auto& objIdx2 : cell2.objects) {
            if (objIdx1 != objIdx2) {
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
    const sf::Vector2f pos = obj->getPosition();
    const float radius = obj->getRadius();
    const sf::Vector2f constraintMin = m_constraintCenter - sf::Vector2f(m_constraintSize.x/2, m_constraintSize.y/2);
    const sf::Vector2f constraintMax = m_constraintCenter + sf::Vector2f(m_constraintSize.x/2, m_constraintSize.y/2);

    // Check if the object is outside the constraint rectangle
    if (obj->getPosition().x - radius < constraintMin.x) {
        obj->setPosition({constraintMin.x + radius, obj->getPosition().y});
    }
    if (obj->getPosition().x + radius > constraintMax.x) {
        obj->setPosition({constraintMax.x - radius, obj->getPosition().y});
    }
    if (obj->getPosition().y - radius < constraintMin.y) {
        obj->setPosition({obj->getPosition().x, constraintMin.y + radius});
    }
    if (obj->getPosition().y + radius > constraintMax.y) {
        obj->setPosition({obj->getPosition().x, constraintMax.y - radius});
    }
}

}

sf::Vector2f vle::Solver::getConstraintSize()
{
    return m_constraintSize;
}

sf::Vector2f vle::Solver::getConstraintCenter()
{
    return m_constraintCenter;
}
