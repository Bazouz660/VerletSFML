/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** main
*/

#include "Engine.hpp"

int main(int argc, char **argv)
{
    vle::Engine engine;

    engine.init();

    try {
        engine.run();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}