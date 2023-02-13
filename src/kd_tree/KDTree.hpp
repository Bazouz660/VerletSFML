/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** KDTree
*/

#ifndef KDTREE_HPP_
    #define KDTREE_HPP_

    #include "common.hpp"
    #include "VerletObject.hpp"

    namespace vle {
        class KdTree
        {
        public:
            KdTree();
            ~KdTree();

            void insert(VerletObject* obj);
            std::vector<VerletObject *> searchInRadius(const sf::Vector2f &point, float radius) const;
            void clear();

        private:
            struct Node
            {
                VerletObject *obj;
                Node *left;
                Node *right;
                int depth;
            };

            Node *root;

            void insert(Node *&node, VerletObject *obj, int depth);
            void clear(Node *node);
            void searchInRadius(Node *node, const sf::Vector2f &point, float radius, std::vector<VerletObject*>& objects) const;
            float distance(const sf::Vector2f &p1, const sf::Vector2f &p2) const;
        };
    }

#endif /* !KDTREE_HPP_ */
