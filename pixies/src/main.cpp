#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cstring>

#include "Context.h"
#include "Shader.h"
#include "InputMonitor.h"
#include "Background.h"
#include "Pixies/PerlinNoisedPlanktonPixies.h"
#include "Pixies/NoisedPlanktonPixies.h"
#include "Pixies/PlanktonPixies.h"

kki::Texture* backgroundFactory(int argc, char* argv[], kki::Context& context){
    if (argc < 2) {
        std::cout << "At least one argument required, possible arguments:"
                  << "\n\tplankton"
                  << "\n\tnoisy"
                  << "\n\tperlin"
                  << "\n\tnaive"
                  << std::endl;
        throw std::invalid_argument("At least 1 arguments required!");
    }

    std::string type(argv[1]);

    int background_scale                    = 3;
    FastNoiseLite::NoiseType    noiseType   = FastNoiseLite::NoiseType_Perlin;
    FastNoiseLite::FractalType  fractalType = FastNoiseLite::FractalType_FBm;
    size_t                      octaves     = 2;

    if (type == "perlin"){
        return new PerlinNoiseBackground(
            static_cast<int>(context.getWidth() / background_scale),
            static_cast<int>(context.getHeight() / background_scale)
            );
    }
    else if (
            type == "noisy" ||
            type == "plankton" ||
            type == "naive"
        ){
        return new NoiseBackground(
            static_cast<int>(context.getWidth() / background_scale),
            static_cast<int>(context.getHeight() / background_scale),
            noiseType,
            fractalType,
            octaves);
    }
    throw std::invalid_argument("Pixie type not supported!");
}

kki::Pixies* pixieFactory(int argc, char* argv[], kki::Texture* background){
    if (argc < 2) {
        std::cout << "At least one argument required, possible arguments:"
                  << "\n\tplankton"
                  << "\n\tnoisy"
                  << "\n\tperlin"
                  << "\n\tnaive"
                  << std::endl;
        throw std::invalid_argument("At least 1 arguments required!");
    }

    std::string type{argv[1]};

    if      (type == "noisy"){
        try{
            if (argc < 7){
                throw std::invalid_argument("");
            }
            unsigned count  = std::stoi(argv[2]);
            float speed     = std::stof(argv[3]);
            float acc       = std::stof(argv[4]);
            float scale     = std::stof(argv[5]);
            float rand      = std::stof(argv[6]);

            NoisedPlanktonPixies::Setup setup{count, speed, acc, scale, rand,true};
            return new NoisedPlanktonPixies(background, setup);
        }
        catch (...){
            std::cout << "6 arguments required, arguments:"
                      << "\n\t1) Type (noisy)"
                      << "\n\t2) Count of pixies        (int)"
                      << "\n\t3) Max speed of pixies    (float)"
                      << "\n\t4) Acceleration of pixies (float)"
                      << "\n\t5) Noise scaling factor   (float)"
                      << "\n\t6) Random noise intensity (float)"
                      << std::endl;
            throw std::invalid_argument("6 arguments required!");
        }
    }
    else if (type == "perlin"){
        try{
            if (argc < 7){
                throw std::invalid_argument("");
            }
            unsigned count  = std::stoi(argv[2]);
            float speed     = std::stof(argv[3]);
            float acc       = std::stof(argv[4]);
            float scale     = std::stof(argv[5]);
            float rand      = std::stof(argv[6]);

            PerlinNoisedPlanktonPixies::Setup setup{count, speed, acc, scale, rand,true};
            return new PerlinNoisedPlanktonPixies(background, setup);
        }
        catch (...){
            std::cout << "6 arguments required, arguments:"
                      << "\n\t1) Type (perlin)"
                      << "\n\t2) Count of pixies        (int)"
                      << "\n\t3) Max speed of pixies    (float)"
                      << "\n\t4) Acceleration of pixies (float)"
                      << "\n\t5) Noise scaling factor   (float)"
                      << "\n\t6) Random noise intensity (float)"
                      << std::endl;
            throw std::invalid_argument("6 arguments required!");
        }
    }
    else if (type == "plankton"){
        try {
            if (argc < 6){
                throw std::invalid_argument("");
            }

            unsigned count  = std::stoi(argv[2]);
            float speed     = std::stof(argv[3]);
            float acc       = std::stof(argv[4]);
            float scale     = std::stof(argv[5]);

            PlanktonPixies::Setup setup{count, speed, acc, scale, true};
            return new PlanktonPixies(background, setup);
        }
        catch (...){
            std::cout << "5 arguments required, arguments:"
                      << "\n\t1) Type (plankton)"
                      << "\n\t2) Count of pixies        (int)"
                      << "\n\t3) Max speed of pixies    (float)"
                      << "\n\t4) Acceleration of pixies (float)"
                      << "\n\t5) Noise scaling factor   (float)"
                      << std::endl;
            throw std::invalid_argument("5 arguments required!");
        }
    }
    else if (type == "naive"){
        try{
            if (argc < 3){
                throw std::invalid_argument("");
            }

            unsigned count  = std::stoi(argv[2]);
            return new NaivePixies(background, count, true);
        }
        catch (...){
            std::cout << "2 argument required, arguments:"
                      << "\n\t1) Type (plankton)"
                      << "\n\t2) Count of pixies        (int)"
                      << std::endl;
            throw std::invalid_argument("2 arguments required!");
        }
    }

    throw std::invalid_argument("Noise type does not exist");
}

/*
Configurations:
 Guzvanje:
    plankton 100000 0.001 0.0015 2.0
 Rings:
    plankton 100000 0.01 0.0075 16.0
 Stripes:
    plankton 250000 0.005 0.0005 4.0
 Silk:
    plankton 250000 0.01 0.0005 4.0
 Phoenix:
    plankton 500000 0.005 0.0005 6.0
 Noisy phoenix:
    noisy 100000 0.005 0.0002 6.0 0.001
 */

int main(int argc, char* argv[])
{
    // Animation parameters
    int width               = 720;
    int height              = 720;

    // Drawing
    bool draw_background    = false;
    bool draw_pixies        = true;
    // Duration of frame
    std::chrono::milliseconds frame_len(30);


    // Window init
    kki::Context window(width, height, "Name");
    kki::InputMonitor input ({GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_R}, window);
    window.bind();

    // Shaders init
    auto shader = std::make_shared<kki::Shader>("../src/shaders/pixies.glsl");
    auto background_shader = std::make_shared<kki::Shader>("../src/shaders/texture.glsl");
    background_shader->use();


    auto noise = backgroundFactory(argc, argv, window);
    kki::Pixies* pixies = pixieFactory(argc, argv, noise);

    while (!window.shouldClose())
    {
        auto begin = std::chrono::steady_clock::now();
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        noise->update();
        if(draw_background) noise->draw(background_shader);

        pixies->updateAll();
        if (draw_pixies) pixies->draw(shader);

        window.swapBuffers();
        auto end = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(frame_len - (end - begin));
        window.resetViewport();
        window.pollEvents();

        if (input[GLFW_KEY_UP]){
            draw_background = true;
        }
        else if (input[GLFW_KEY_DOWN]){
            draw_background = false;
        }
        if (input[GLFW_KEY_RIGHT]){
            draw_pixies = true;
        }
        else if (input[GLFW_KEY_LEFT]){
            draw_pixies = false;
        }
        if (input[GLFW_KEY_R]){
            pixies->resetPositions();
        }
    }
    return 0;
}


