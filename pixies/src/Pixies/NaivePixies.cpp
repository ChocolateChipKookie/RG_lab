//
// Created by kookie on 11. 01. 2021..
//

#include "NaivePixies.h"
#include <glm/glm.hpp>

NaivePixies::NaivePixies(kki::Texture* background, size_t count, bool random_start)
    :   Pixies(count, random_start), _background(background)
{
    _data.reserve(count);
    for (unsigned i = 0; i < count; ++i){
        _data.push_back({0.01, rng.randomReal(0.f, 2 * PI)});
    }
}

void NaivePixies::update(size_t i)
{
    auto& pos = positions[i];
    auto& col = colours[i];
    auto& mov = _data[i];

    // Update direction
    float val = _background->at(static_cast<size_t>((pos.x + 1.f) * 499.f / 2.f), static_cast<size_t>((pos.y + 1.f) * 499.f / 2.f))[0];
    mov.direction = val * noise_mapping;


    // Update position
    float d_x = glm::cos(mov.direction) * mov.speed;
    float d_y = glm::sin(mov.direction) * mov.speed;

    pos.x = pos.x + d_x;
    pos.y = pos.y + d_y;

    // If they are outside, reassign the position
    if ( pos.x < -0.99f || pos.y > 0.99f){
        pos = {rng.randomReal(-0.99f, 0.99f), rng.randomReal(-0.99f, 0.99f)};
    }

    // Update colour
    auto red = [](float x){return std::min(std::max(std::abs(6 * x - 3) - 1, 0.f), 0.99f);};
    auto green = [](float x){return std::min(std::max(-std::abs(6 * x - 2) + 2, 0.f), 0.99f);};
    auto blue = [](float x){return std::min(std::max(-std::abs(6 * x - 4) + 2, 0.f), 0.99f);};

    col = {red(val), green(val), blue(val)};
}

