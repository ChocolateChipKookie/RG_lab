//
// Created by kookie on 11. 01. 2021..
//

#ifndef PIXIES_PERLINNOISE_H
#define PIXIES_PERLINNOISE_H

#include "util/random.h"
#include <numeric>
#include <algorithm>

class PerlinNoise{
public:

    explicit PerlinNoise(const std::vector<int>& permutation){
        assert(permutation.size() == 256);
        std::copy(permutation.begin(), permutation.end(), p);
        std::copy(permutation.begin(), permutation.end(), p + 256);
    }

    PerlinNoise(){
        kki::Random& rng = kki::rng::tl;
        std::iota(p, p+256, 0);
        std::shuffle(p, p+256, rng.get_engine());
        std::copy(p, p+256, p+256);
    }

    double noise(double x, double y, double z) {
        int X = static_cast<int>(floor(x)) & 255;
        int Y = static_cast<int>(floor(y)) & 255;
        int Z = static_cast<int>(floor(z)) & 255;

        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        double u = fade(x);
        double v = fade(y);
        double w = fade(z);

        int A  = p[X  ] + Y;
        int AA = p[A]   + Z;
        int AB = p[A+1] + Z;
        int B  = p[X+1] + Y;
        int BA = p[B]   + Z;
        int BB = p[B+1] + Z;

        double g1 = grad(p[AA  ], x  , y  , z   );
        double g2 = grad(p[BA  ], x-1, y  , z   );
        double g3 = grad(p[AB  ], x  , y-1, z   );
        double g4 = grad(p[BB  ], x-1, y-1, z   );
        double g5 = grad(p[AA+1], x  , y  , z-1 );
        double g6 = grad(p[BA+1], x-1, y  , z-1 );
        double g7 = grad(p[AB+1], x  , y-1, z-1 );
        double g8 = grad(p[BB+1], x-1, y-1, z-1 );

        return trilinear_interpolation(u, v, w, g1, g2, g3, g4, g5, g6, g7, g8);
    }

private:

    int p[512]{};

    static double trilinear_interpolation(double x, double y, double z, double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8){
        double x1 = lerp(x, v1, v2);
        double x2 = lerp(x, v3, v4);
        double x3 = lerp(x, v5, v6);
        double x4 = lerp(x, v7, v8);

        double y1 = lerp(y, x1, x2);
        double y2 = lerp(y, x3, x4);

        double z1 = lerp(z, y1, y2);

        return z1;
    }

    static double fade(double t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    static double lerp(double t, double a, double b) {
        return a + t * (b - a);
    }

    static double grad(int hash, double x, double y, double z) {
        int h = hash & 15;
        double u = h<8 ? x : y;
        double v = y;
        if (h >=4 ){
            v = z;
            if (h==12||h==14){
                v = x;
            }
        }

        return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
    }
};

#endif //PIXIES_PERLINNOISE_H
