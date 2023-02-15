/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Solver
*/

#ifndef SOLVER_HPP_
    #define SOLVER_HPP_

    #include "common.hpp"
    #include "VerletObject.hpp"
    #include "Link.hpp"
    #include "Grid.hpp"
    #include <functional>

    namespace vle {
        class Solver
        {
            public:
                Solver(std::vector<std::unique_ptr<VerletObject>>& objects,
                    std::vector<std::unique_ptr<Link>>& links, unsigned int rate);
                void update();
                sf::Vector2f getConstraintCenter();
                sf::Vector2f getConstraintSize();
                void setSimulationUpdateRate(unsigned int rate);
                void drawGrid(sf::RenderTarget& target);

            private:
                void solveCollision(VerletObject& obj1, VerletObject& obj2);
                void applyConstraint();
                void applyGravity();
                void updatePosition(float dt);
                void solveCollisions();
                void updatePartionning();
                float getStepDt() const;
                float getTime() const;
                void checkCellsCollisions(vle::Grid::Cell& cell1, vle::Grid::Cell& cell2);

                void parallelCollisionCheck();

                void threadFunction(int startRow, int endRow);

            private:
                std::vector<std::unique_ptr<VerletObject>>& m_objects;
                std::vector<std::unique_ptr<Link>>& m_links;
                sf::Vector2f m_constraintCenter = {960.0f, 500.0f};
                sf::Vector2f m_constraintSize = {1800.0f, 900.0f};
                //KdTree m_tree;
                Grid m_grid;
                int m_substeps = 8;
                float m_frameDt;
                float m_time;
        };
    }
#endif /* !SOLVER_HPP_ */
