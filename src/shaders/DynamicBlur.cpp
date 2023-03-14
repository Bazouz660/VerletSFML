/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** DynamicBlur
*/

#include "DynamicBlur.hpp"

sf::Sprite& Blur::apply(const sf::Texture &texture)
{
    // Retrieve texture content and downscale it
    sf::Sprite sprite(texture);
    uint32_t current_buffer = 0;
    m_textures[current_buffer].draw(sprite);
    // current_buffer = blurIteration(current_buffer, 1);
    for (int32_t i(0); i < m_iterations; i += 2)
    {
        current_buffer = blurPass(current_buffer, i);
    }
    for (int32_t i(m_iterations); i >= 0; i -= 3)
    {
        current_buffer = blurPass(current_buffer, i);
    }
    m_textures[current_buffer].display();
    m_result.setTexture(m_textures[current_buffer].getTexture());
    return m_result;
}

Blur::Blur(sf::Vector2u render_size, int32_t iterations)
    : m_render_size(render_size), m_iterations(iterations)
{
    createTextures();
    createShaders();
}

void Blur::createTextures()
{
    // Initialize textures
    m_textures[0].create(m_render_size.x, m_render_size.y);
    m_textures[1].create(m_render_size.x, m_render_size.y);
    m_textures[0].setSmooth(true);
    m_textures[1].setSmooth(true);
    // Update clear
    m_textures[0].display();
    m_textures[1].display();
}

void Blur::createShaders()
{
    m_horizontal.loadFromMemory(VERTEX_SHADER_SRC, FRAG_X_SHADER_SRC);
    m_vertical.loadFromMemory(VERTEX_SHADER_SRC, FRAG_Y_SHADER_SRC);
    // Set pixel steps in shader
    m_horizontal.setUniform("WIDTH", static_cast<float>(m_render_size.x));
    m_vertical.setUniform("HEIGHT", static_cast<float>(m_render_size.y));
}

void Blur::clear(uint32_t texture_id)
{
    m_textures[texture_id].clear(sf::Color::Black);
}

void Blur::draw(const sf::Sprite &sprite, uint32_t dest_buffer, const sf::Shader &shader)
{
    clear(dest_buffer);
    m_textures[dest_buffer].draw(sprite, &shader);
}

void Blur::draw(const sf::Sprite &sprite, uint32_t dest_buffer)
{
    clear(dest_buffer);
    m_textures[dest_buffer].draw(sprite);
}

const sf::Texture& Blur::getTexture(uint32_t source_buffer)
{
    return m_textures[source_buffer].getTexture();
}

uint32_t Blur::blurPass(uint32_t source_buffer, int32_t downscale)
{
    // Initialize scales and rectangle
    const auto inv_scale = static_cast<float>(1 << downscale);
    const float scale = 1.0f / inv_scale;
    const int32_t current_pass_size_x = m_render_size.x >> downscale;
    const int32_t current_pass_size_y = m_render_size.y >> downscale;

    // Draw from source to target with separate blur passes
    sf::Sprite sprite;
    sprite.setScale(scale, scale);
    sprite.setTexture(getTexture(source_buffer));
    draw(sprite, !source_buffer);
    sprite.setScale(1.0f, 1.0f);
    sprite.setTexture(getTexture(!source_buffer));
    sprite.setTextureRect({0, 0, current_pass_size_x, current_pass_size_y});
    draw(sprite, source_buffer, m_horizontal);
    sprite.setTexture(getTexture(source_buffer));
    draw(sprite, !source_buffer, m_vertical);

    const float safe_scale = 1.0f;
    sprite.setScale(inv_scale * safe_scale, inv_scale * safe_scale);
    sprite.setTexture(getTexture(!source_buffer));
    draw(sprite, source_buffer);
    return source_buffer;
}
