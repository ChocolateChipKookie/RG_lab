//
// Created by kookie on 11. 01. 2021..
//

#include "Background.h"

void NoiseBackground::update()
{
    float x = 0.f;
    float x_inc = 2.f;
    float y = 0.f;
    float y_inc = 2.f;

    Texture& tex = *this;

    for (unsigned y_i = 0; y_i < height; y_i++){
        for (unsigned x_i = 0; x_i < width; x_i++){
            float val = noise.GetNoise(x, y, current_z);
            tex(x_i, y_i) = {val, val, val};
            x += x_inc;
        }
        y += y_inc;
        x = 0.f;
    }

    changed = true;
    current_z += 0.5;
}

void PerlinNoiseBackground::update()
{
    float x = 0.f;
    float x_inc = 0.02f;
    float y = 0.f;
    float y_inc = 0.02f;

    Texture& tex = *this;

    for (unsigned y_i = 0; y_i < height; y_i++){
        for (unsigned x_i = 0; x_i < width; x_i++){
            float val = noise.noise(x, y, current_z);
            tex(x_i, y_i) = {val, val, val};
            x += x_inc;
        }
        y += y_inc;
        x = 0.f;
    }

    changed = true;
    current_z += 0.002;
}
