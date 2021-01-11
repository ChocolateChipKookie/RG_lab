//
// Created by kookie on 11. 01. 2021..
//

#include "NoisedPlanktonPixies.h"
#include "../util/math.h"
#include <glm/glm.hpp>

NoisedPlanktonPixies::NoisedPlanktonPixies(kki::Texture* background, size_t count, float max_speed, float acceleration, float noise_scale,
    float random_noise, bool random_start)
    : Pixies(count, random_start),
      _background(background),
      _noise_mapping(noise_scale * PI),
      _max_speed(max_speed),
      _acceleration(acceleration),
      _random_noise(random_noise)
{
    _speed.reserve(count);
    for (unsigned i = 0; i < count; ++i){
        _speed.emplace_back(0.f, 0.f);
    }
}
NoisedPlanktonPixies::NoisedPlanktonPixies(kki::Texture* background, const NoisedPlanktonPixies::Setup& setup)
    :   NoisedPlanktonPixies(background, setup.count, setup.max_speed, setup.acceleration, setup.noise_scale, setup.random_noise, setup.random_start)
{}
void NoisedPlanktonPixies::update(size_t i)
{
    auto& pos = positions[i];
    auto& col = colours[i];
    auto& mov = _speed[i];

    // Update direction
    float val =
        _background->at(
            static_cast<size_t>((pos.x + 1.f) * _background->getWidth() / 2.f),
            static_cast<size_t>((pos.y + 1.f) * _background->getHeight() / 2.f)
        )[0];

    float angle = val * _noise_mapping;

    glm::vec2 d_v{glm::cos(angle), glm::sin(angle)};

    mov += d_v * _acceleration;
    float speed = glm::length(mov);
    if (speed > _max_speed){
        mov = glm::normalize(mov) * _max_speed;
        speed = _max_speed;
    }

    speed /= _max_speed;

    // Update position
    pos += mov;
    pos += glm::vec2{rng.randomGaussian(0.f, _random_noise), rng.randomGaussian(0.f, _random_noise)};

    // If they are outside, reassign the position
    if ( !kki::inRange(-0.99f, 0.99f, pos.x) || !kki::inRange(-0.99f, 0.99f, pos.y)){
    // while ( pos.x * pos.x + pos.y * pos.y > 0.95f){
        pos = {rng.randomReal(-0.99f, 0.99f), rng.randomReal(-0.99f, 0.99f)};
    }

    // Update colour
    auto red   = [](float x){return std::min(std::max( std::abs(6 * x - 3) - 1, 0.f), 0.99f);};
    auto green = [](float x){return std::min(std::max(-std::abs(6 * x - 2) + 2, 0.f), 0.99f);};
    auto blue  = [](float x){return std::min(std::max(-std::abs(6 * x - 4) + 2, 0.f), 0.99f);};

    col = {red(speed), green(speed), blue(speed)};
}
