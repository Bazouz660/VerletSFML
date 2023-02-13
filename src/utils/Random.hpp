/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Random
*/

#ifndef RANDOM_HPP_
    #define RANDOM_HPP_

    #include <ctime>
    #include <cstdlib>

    namespace numberGenerator {
        int between(int min, int max) {
            static bool seeded = false;
            if (!seeded) {
                srand(time(0));
                seeded = true;
            }
            return min + rand() % (max - min + 1);
        }
    }

#endif /* !RANDOM_HPP_ */
