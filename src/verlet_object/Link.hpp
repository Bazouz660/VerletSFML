/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Link
*/

#ifndef LINK_HPP_
    #define LINK_HPP_

    #include "common.hpp"
    #include "VerletObject.hpp"

    namespace vle {
        class Link {
            public:
                Link(VerletObject& obj1, VerletObject& obj2, float targetDist);
                ~Link();
                void apply();

            private:
                VerletObject& m_obj1;
                VerletObject& m_obj2;
                float m_targetDist;
        };
    }

#endif /* !LINK_HPP_ */
