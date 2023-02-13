/*
** EPITECH PROJECT, 2023
** VerletSFML
** File description:
** Grid
*/

#include "Grid.hpp"

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
    m_dimensions = dimensions / cellSize;
    m_cellSize = cellSize;
    m_nbCells = m_dimensions.x * m_dimensions.y;

    for (int i = 0; i < m_nbCells + 1; i++) {
        m_cells.push_back(std::unique_ptr<Cell>(new Cell));
    }

    for (int i = 0; i < m_nbCells; i++) {
        m_cells.at(i)->objects.clear();
        m_cells.at(i)->size = cellSize;
        m_cells.at(i)->position = index1dTo2d(i) * cellSize;
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
    for (int i = 0; i < m_nbCells; i++)
        m_cells.at(i)->objects.clear();
}

sf::Vector2u vle::Grid::getDimensions() const
{
    return m_dimensions;
}

void vle::Grid::insert(VerletObject* object)
{
    sf::Vector2i position = sf::Vector2i(object->getPosition());

    position = {position.x / (int)m_cellSize, position.y / (int)m_cellSize};

    //std::cout << "Inserting at position: x" << position.x << ", y" << position.y << std::endl;

    if (position.x > 0 && position.y > 0 && position.x < (m_dimensions.x * m_cellSize)
        && position.y < (m_dimensions.y * m_cellSize)) {
        m_cells.at(index2dTo1d(sf::Vector2u(position)))->objects.push_back(object);
    }
}

vle::Grid::Cell& vle::Grid::get(const sf::Vector2u &position) const
{
    return *m_cells.at(index2dTo1d(position)).get();
}

vle::Grid::Cell& vle::Grid::get(int index) const
{
    return *m_cells.at(index).get();
}

std::vector<vle::VerletObject*> vle::Grid::search(sf::Vector2u position) const
{
    std::vector<VerletObject*> result;
    std::vector<VerletObject*> tmp;

    tmp = m_cells.at(index2dTo1d(position))->objects;
    result.insert(result.end(), tmp.begin(), tmp.end());

    //std::cout << position.x << " " << position.y << std::endl;

    if (position.x + 1 < m_dimensions.x) {
        tmp = m_cells.at(index2dTo1d({position.x + 1, position.y}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.x + 1 < m_dimensions.x && position.y + 1 < m_dimensions.y) {
        tmp = m_cells.at(index2dTo1d({position.x + 1, position.y + 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.x > 0 && position.y > 0) {
        tmp = m_cells.at(index2dTo1d({position.x - 1, position.y - 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.y + 1 < m_dimensions.y) {
        tmp = m_cells.at(index2dTo1d({position.x, position.y + 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.x + 1 < m_dimensions.x && position.y > 0) {
        tmp = m_cells.at(index2dTo1d({position.x + 1, position.y - 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.x > 0 && position.y + 1 < m_dimensions.y) {
        tmp = m_cells.at(index2dTo1d({position.x - 1, position.y + 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }


    if (position.x > 0) {
        tmp = m_cells.at(index2dTo1d({position.x - 1, position.y}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    if (position.y > 0) {
        tmp = m_cells.at(index2dTo1d({position.x, position.y - 1}))->objects;
        result.insert(result.end(), tmp.begin(), tmp.end());
    }

    return result;
}

vle::Grid::~Grid()
{
}
