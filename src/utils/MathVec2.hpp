/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** mathVec2
*/

#ifndef MATHVEC2_HPP_
    #define MATHVEC2_HPP_

    #include "common.hpp"

    namespace mathVec2 {
        template<typename T>
        T length(sf::Vector2<T> vec) {
            return std::sqrt(vec.x * vec.x + vec.y * vec.y);
        }
    }
#endif /* !MATHVEC2_HPP_ */
