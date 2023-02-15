/*
** EPITECH PROJECT, 2023
** VerletSFML
** File description:
** Grid
*/

#ifndef GRID_HPP_
    #define GRID_HPP_

    #include "common.hpp"
    #include "VerletObject.hpp"

    namespace vle {
        class Grid {
            public:
                class Cell {
                    public:
                        Cell(sf::Vector2u pos, unsigned int size);
                        std::vector<VerletObject*>objects;
                        sf::Vector2u position;
                        unsigned int size;
                };
                Grid(const sf::Vector2u& dimensions, unsigned int cellSize);
                ~Grid();
                void clear();
                Cell& get(const sf::Vector2u &position) const;
                Cell& get(int index) const;
                void insert(VerletObject* object);
                unsigned int getCellSize() const;
                unsigned int getNbCells() const;
                sf::Vector2u getDimensions() const;
                std::vector<VerletObject*> search(sf::Vector2u position) const;
                unsigned int index2dTo1d(const sf::Vector2u& index) const;
                sf::Vector2u index1dTo2d(const unsigned int index) const;
                const std::vector<std::unique_ptr<std::vector<std::unique_ptr<Cell>>>>& getCells() const;

            private:
                unsigned int m_cellSize;
                unsigned int m_nbCells;
                sf::Vector2u m_dimensions;
                std::vector<std::unique_ptr<std::vector<std::unique_ptr<Cell>>>> m_cells;
        };
    }

#endif /* !GRID_HPP_ */
