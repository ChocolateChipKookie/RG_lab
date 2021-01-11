//
// Created by kookie on 11. 01. 2021..
//

#ifndef PIXIES_PERLINNOISEDPLANKTONPIXIES_H
#define PIXIES_PERLINNOISEDPLANKTONPIXIES_H

#include "Pixies.h"
#include "../Background.h"

class PerlinNoisedPlanktonPixies : public kki::Pixies{
private:
    kki::Random& rng = kki::rng::global;
    const float PI = 3.141592;
    const float _noise_mapping;

    const float _max_speed;
    const float _acceleration;
    const float _random_noise;

    std::vector<glm::vec2> _speed;
    kki::Texture* _background;
public:

    struct Setup{
        size_t count;
        float max_speed, acceleration, noise_scale, random_noise;
        bool random_start;
    };

    explicit PerlinNoisedPlanktonPixies(kki::Texture* background, size_t count, float max_speed, float acceleration, float noise_scale, float random_noise, bool random_start);

    explicit PerlinNoisedPlanktonPixies(kki::Texture* background, const Setup& setup);

    void update(size_t i) override;
};


#endif //PIXIES_PERLINNOISEDPLANKTONPIXIES_H
