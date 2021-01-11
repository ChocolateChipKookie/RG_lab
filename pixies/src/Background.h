//
// Created by kookie on 11. 01. 2021..
//

#ifndef PIXIES_BACKGROUND_H
#define PIXIES_BACKGROUND_H

#include "Texture.h"

#include <FastNoiseLite.h>
#include "PerlinNoise.h"

class NoiseBackground : public kki::Texture{
    FastNoiseLite noise;
    float current_z = 0.f;

public:
    NoiseBackground(int width, int height, FastNoiseLite::NoiseType noiseType, FastNoiseLite::FractalType fractalType, size_t octaves)
        :   Texture(width, height)
    {
        noise.SetNoiseType(noiseType);
        noise.SetFractalType(fractalType);
        noise.SetFractalOctaves(octaves);
    }

    void update() override;

};

class PerlinNoiseBackground : public kki::Texture{
    PerlinNoise noise;
    float current_z = 0.f;

public:
    PerlinNoiseBackground(int width, int height)
        :   Texture(width, height)
    {
    }

    void update() override;
};

#endif //PIXIES_BACKGROUND_H
