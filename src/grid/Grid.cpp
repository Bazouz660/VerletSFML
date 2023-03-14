/*
** EPITECH PROJECT, 2023
** VerletSFML
** File description:
** Grid
*/

#include "Grid.hpp"

vle::Grid::Cell::Cell(sf::Vector2u pos, unsigned int size)
: position(pos), size(size)
{
}

unsigned int vle::Grid::index2dTo1d(const sf::Vector2u& index) const
{
    return (index.y * m_dimensions.y) + index.x;
}

sf::Vector2u vle::Grid::index1dTo2d(const unsigned int index) const
{
    unsigned int column = index % m_dimensions.y;
    unsigned int row = index / m_dimensions.x;

    return {row, column};
}

vle::Grid::Grid(const sf::Vector2u& dimensions, unsigned int cellSize)
{
    m_cellSize = cellSize * 2;
    m_dimensions = dimensions / m_cellSize;
    m_nbCells = m_dimensions.x * m_dimensions.y;

    for (int x = 0; x < m_dimensions.x; x++) {
        m_cells.push_back(std::unique_ptr<std::vector<std::unique_ptr<Cell>>>(new std::vector<std::unique_ptr<Cell>>));
        for (int y = 0; y < m_dimensions.y; y++) {
            m_cells.at(x)->push_back(std::unique_ptr<Cell>(new Cell({x, y}, m_cellSize)));
        }
    }
}

unsigned int vle::Grid::getCellSize() const
{
    return m_cellSize;
}

unsigned int vle::Grid::getNbCells() const
{
    return m_nbCells;
}

void vle::Grid::clear()
{
    for (auto& cellsX : m_cells) {
        for (auto& cellsY : *cellsX.get()) {
            cellsY->objects.clear();
        }
    }
}

sf::Vector2u vle::Grid::getDimensions() const
{
    return m_dimensions;
}

void vle::Grid::insert(VerletObject* object)
{
    sf::Vector2i position = sf::Vector2i(object->getPosition());

    position = {position.x / (int)m_cellSize, position.y / (int)m_cellSize};

    if (position.x < 0 || position.y < 0 || position.x >= m_dimensions.x || position.y >= m_dimensions.y)
        return;

    m_cells.at(position.x)->at(position.y)->objects.push_back(object);
}

vle::Grid::Cell& vle::Grid::get(const sf::Vector2u &position) const
{
    return *m_cells.at(position.x)->at(position.y).get();
}

const std::vector<std::unique_ptr<std::vector<std::unique_ptr<vle::Grid::Cell>>>>& vle::Grid::getCells() const
{
    return m_cells;
}

vle::Grid::~Grid()
{
}
