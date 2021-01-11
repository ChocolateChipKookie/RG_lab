//
// Created by kookie on 11. 01. 2021..
//

#ifndef PIXIES_NAIVEPIXIES_H
#define PIXIES_NAIVEPIXIES_H

#include "Pixies.h"
#include "../Background.h"

class NaivePixies : public kki::Pixies{
private:
    kki::Random& rng = kki::rng::global;
    const float PI = 3.141592;
    const float noise_mapping = 4 * PI;

    struct pixieData{
        float speed = 0;
        float direction = 0;
    };

    std::vector<pixieData> _data;
    kki::Texture* _background;
public:
    explicit NaivePixies(kki::Texture* background, size_t count, bool random_start);

    void update(size_t i) override;
};

#endif //PIXIES_NAIVEPIXIES_H
