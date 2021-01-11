//
// Created by kookie on 11. 01. 2021..
//

#ifndef PIXIES_PLANKTONPIXIES_H
#define PIXIES_PLANKTONPIXIES_H

#include "Pixies.h"
#include "NaivePixies.h"

class PlanktonPixies : public kki::Pixies{
private:
    kki::Random& rng = kki::rng::global;
    const float PI = 3.141592;
    const float _noise_mapping;

    const float _max_speed;
    const float _acceleration;

    std::vector<glm::vec2> _speed;
    kki::Texture* _background;
public:

    struct Setup{
        size_t count;
        float max_speed, acceleration, noise_scale;
        bool random_start;
    };

    explicit PlanktonPixies(kki::Texture* background, size_t count, float max_speed, float acceleration, float noise_scale, bool random_start);

    explicit PlanktonPixies(kki::Texture* background, const Setup& setup);


    void update(size_t i) override;
};


#endif //PIXIES_PLANKTONPIXIES_H
