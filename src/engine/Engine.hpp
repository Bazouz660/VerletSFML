/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Engine
*/

#ifndef ENGINE_HPP_
    #define ENGINE_HPP_

    #include "common.hpp"
    #include "VerletObject.hpp"
    #include "Solver.hpp"
    #include <vector>

    namespace vle {
        class Engine {
            public:
                Engine();
                ~Engine();
                void run();
                void init();

            private:
                std::unique_ptr<sf::RenderWindow> m_window;
                sf::Event m_event;
                sf::Clock m_clock;
                sf::Time m_previousTime;
                std::vector<std::unique_ptr<VerletObject>> m_objects;
                std::vector<std::unique_ptr<Link>> m_links;
                float m_deltaTime;

                sf::Text m_fpsText;
                sf::Text m_objNbText;
                sf::Font m_font;

                Solver m_solver;

            private:
                void computeFrameTime();
                void pollEvents();
                int getFps();
        };
    }

#endif /* !ENGINE_HPP_ */
