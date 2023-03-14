/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** DynamicBlur
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

constexpr char VERTEX_SHADER_SRC[] =
    "void main()                                                  \
{                                                             \
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0; \
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;   \
}";

constexpr char FRAG_X_SHADER_SRC[] =
    "uniform sampler2D texture;                                  \
uniform float WIDTH;                                         \
float WIDTH_STEP = 1.0 / WIDTH;                              \
void main()                                                  \
{                                                            \
	vec2 pos = gl_TexCoord[0].xy;                            \
	vec2 offset = vec2(WIDTH_STEP, 0.0);                     \
	vec4 color = texture2D(texture, pos) * 0.383;            \
	color += texture2D(texture, pos + offset * 1.5) * 0.242; \
	color += texture2D(texture, pos + offset * 2.5) * 0.061; \
	color += texture2D(texture, pos + offset * 3.5) * 0.006; \
	color += texture2D(texture, pos - offset * 1.5) * 0.242; \
	color += texture2D(texture, pos - offset * 2.5) * 0.061; \
	color += texture2D(texture, pos - offset * 3.5) * 0.006; \
	gl_FragColor = vec4(color.xyz, 1.0);                     \
}";

constexpr char FRAG_Y_SHADER_SRC[] =
    "uniform sampler2D texture;                                  \
uniform float HEIGHT;                                        \
float HEIGHT_STEP = 1.0 / HEIGHT;                            \
void main()                                                  \
{                                                            \
	vec2 pos = gl_TexCoord[0].xy;                            \
	vec2 offset = vec2(0.0, HEIGHT_STEP);                    \
	vec4 color = texture2D(texture, pos) * 0.383;            \
	color += texture2D(texture, pos + offset * 1.5) * 0.242; \
	color += texture2D(texture, pos + offset * 2.5) * 0.061; \
	color += texture2D(texture, pos + offset * 3.5) * 0.006; \
	color += texture2D(texture, pos - offset * 1.5) * 0.242; \
	color += texture2D(texture, pos - offset * 2.5) * 0.061; \
	color += texture2D(texture, pos - offset * 3.5) * 0.006; \
	gl_FragColor = vec4(color.xyz, 1.0);                     \
}";

class Blur {
public:
    Blur(sf::Vector2u render_size, int32_t iterations);
    sf::Sprite &apply(const sf::Texture &texture);

private:
    sf::Vector2i m_render_size;
    int32_t m_iterations;
    sf::RenderTexture m_textures[2];

    // Shaders
    sf::Shader m_horizontal;
    sf::Shader m_vertical;
    sf::Sprite m_result;

    // Methods
    void createTextures();
    void createShaders();
    void clear(uint32_t texture_id);
    void draw(const sf::Sprite &sprite, uint32_t dest_buffer, const sf::Shader &shader);
    void draw(const sf::Sprite &sprite, uint32_t dest_buffer);
    const sf::Texture &getTexture(uint32_t source_buffer);
    uint32_t blurPass(uint32_t source_buffer, int32_t downscale);
};