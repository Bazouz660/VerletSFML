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
    #include "KDTree.hpp"
    #include "Grid.hpp"

    namespace vle {
        class Solver
        {
            public:
                Solver(std::vector<std::unique_ptr<VerletObject>>& objects,
                    std::vector<std::unique_ptr<Link>>& links, unsigned int rate);
                void update();
                float getConstraintRadius();
                sf::Vector2f getConstraintPosition();
                void setSimulationUpdateRate(unsigned int rate);

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

            private:
                std::vector<std::unique_ptr<VerletObject>>& m_objects;
                std::vector<std::unique_ptr<Link>>& m_links;
                sf::Vector2f m_constraintCenter = {800.0f, 450.0f};
                //KdTree m_tree;
                Grid m_grid;
                float m_constraintRadius = 400.0f;
                int m_substeps = 8;
                float m_frameDt;
                float m_time;
        };
    }
#endif /* !SOLVER_HPP_ */
