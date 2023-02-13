/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** KDTree
*/

#include "KDTree.hpp"
#include "MathVec2.hpp"
#include "VerletObject.hpp"

vle::KdTree::KdTree()
    : root(NULL)
{
}

vle::KdTree::~KdTree()
{
    clear();
}

void vle::KdTree::insert(VerletObject *obj)
{
    insert(root, obj, 0);
}

std::vector<vle::VerletObject *> vle::KdTree::searchInRadius(const sf::Vector2f &point, float radius) const
{
    std::vector<VerletObject *> objects;
    searchInRadius(root, point, radius, objects);
    return objects;
}

void vle::KdTree::insert(Node *&node, VerletObject *obj, int depth)
{
    if (!node)
    {
        node = new Node;
        node->obj = obj;
        node->left = node->right = nullptr;
        node->depth = depth;
        return;
    }

    int axis = depth % 2;
    if ((axis == 0 && obj->getPosition().x < node->obj->getPosition().x) ||
        (axis == 1 && obj->getPosition().y < node->obj->getPosition().y))
    {
        insert(node->left, obj, depth + 1);
    }
    else
    {
        insert(node->right, obj, depth + 1);
    }
}

void vle::KdTree::clear()
{
    clear(root);
    root = NULL;
}

void vle::KdTree::clear(Node *node)
{
    if (!node)
        return;

    clear(node->left);
    clear(node->right);
    delete node;
}

void vle::KdTree::searchInRadius(Node *node, const sf::Vector2f &point, float radius, std::vector<VerletObject *> &objects) const
{
    if (!node)
        return;

    float d = distance(node->obj->getPosition(), point);
    if (d <= radius + node->obj->getRadius())
        objects.push_back(node->obj);

    int axis = node->depth % 2;
    if ((axis == 0 && point.x - radius < node->obj->getPosition().x) ||
        (axis == 1 && point.y - radius < node->obj->getPosition().y))
    {
        searchInRadius(node->left, point, radius, objects);
    }

    if ((axis == 0 && point.x + radius > node->obj->getPosition().x) ||
        (axis == 1 && point.y + radius > node->obj->getPosition().y))
    {
        searchInRadius(node->right, point, radius, objects);
    }
}

float vle::KdTree::distance(const sf::Vector2f &p1, const sf::Vector2f &p2) const
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
