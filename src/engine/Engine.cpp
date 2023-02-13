/*
** EPITECH PROJECT, 2023
** verletPhysics
** File description:
** Engine
*/

#include "Engine.hpp"
#include "Solver.hpp"
#include "Random.hpp"

namespace vle {

Engine::Engine()
: m_solver(m_objects, m_links, 60)
{
    m_font.loadFromFile("assets/font/debug_font.ttf");

    m_fpsText.setFont(m_font);
    m_fpsText.setPosition(0, 0);
    m_fpsText.setOutlineThickness(1);
    m_fpsText.setOutlineColor(sf::Color::Black);
    m_fpsText.setString("FPS: 0");

    m_objNbText = m_fpsText;
    m_objNbText.setPosition(0, 50);
    m_fpsText.setString("Objects: 0");
}

Engine::~Engine()
{
}

void Engine::init()
{
    sf::ContextSettings context;
    context.antialiasingLevel = 8;

    m_window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(1920, 1080),
        std::string("Game"), sf::Style::Default, context));

    m_window->setVerticalSyncEnabled(true);



    int nb_links = 20;

    for (int i = 0; i < nb_links; i++)
        m_objects.push_back(std::unique_ptr<VerletObject>(new VerletObject(5, {500.f + (i * 30), 500}, 15, sf::Color::White)));

    m_objects.at(0)->setFixed(true);
    m_objects.at(nb_links - 1)->setFixed(true);

    for (int i = 0; i < nb_links - 1; i++)
        m_links.push_back(std::unique_ptr<Link>(new Link(*m_objects.at(i).get(), *m_objects.at(i + 1).get(), 32)));
}

void Engine::computeFrameTime()
{
    static bool initialized = false;
    sf::Time currentTime = m_clock.getElapsedTime();

    if (!initialized) {
        initialized = true;
        m_previousTime = currentTime;
    }

    m_deltaTime = currentTime.asSeconds() - m_previousTime.asSeconds();
    m_previousTime = currentTime;
}

void Engine::pollEvents()
{
    while (m_window->pollEvent(m_event)) {
        if (m_event.type == sf::Event::Closed)
            m_window->close();
    }
}

int Engine::getFps()
{
    return (1.0 / m_deltaTime);
}

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * 3.14159265359);
    const float b = sin(t + 0.66f * 2.0f * 3.14159265359);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

void Engine::run()
{
    static float spawnerTick = m_clock.getElapsedTime().asSeconds();
    static float fpsRefreshTick = -1;

    while (m_window->isOpen()) {
        m_window->clear(sf::Color(50, 50, 50, 255));
        this->pollEvents();
        if (!m_window->isOpen())
            break;
        computeFrameTime();

        if (m_clock.getElapsedTime().asSeconds() > fpsRefreshTick + 0.1) {
            fpsRefreshTick = m_clock.getElapsedTime().asSeconds();
            m_fpsText.setString("FPS: " + std::to_string(getFps()));
        }


        if (m_clock.getElapsedTime().asSeconds() > spawnerTick + 0.02 && m_deltaTime < 0.030) {
            spawnerTick = m_clock.getElapsedTime().asSeconds();
            static float angle = 0;

            m_objects.push_back(std::unique_ptr<VerletObject>(new VerletObject(5, {700, 200},
                numberGenerator::between(2, 15), getRainbow(spawnerTick))));
            m_objects.back()->accelerate({std::cos(spawnerTick) * 23000000 * m_deltaTime, std::sin(spawnerTick) * 20000000 * m_deltaTime});
            m_objNbText.setString("Objects: " + std::to_string(m_objects.size()));
            angle += 0.0174533;
        }

        m_solver.update();

        sf::CircleShape solverConstraintShape;
        solverConstraintShape.setFillColor(sf::Color::Black);
        solverConstraintShape.setPointCount(100);
        solverConstraintShape.setPosition(m_solver.getConstraintPosition());
        solverConstraintShape.setRadius(m_solver.getConstraintRadius());
        solverConstraintShape.setOrigin(solverConstraintShape.getRadius(), solverConstraintShape.getRadius());

        m_window->draw(solverConstraintShape);

        for (auto& object : m_objects) {
            object->draw(*m_window);
        }

        m_window->draw(m_fpsText);
        m_window->draw(m_objNbText);

        m_window->display();
    }
}

}