//
// Created by kookie on 02/04/2020.
//

#include "image_loader.h"
#include "graphics_error.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


meg::image_loader::image_data meg::image_loader::load_image(const std::string &image_path, bool flip_on_load, int desired_channels) {
    stbi_set_flip_vertically_on_load(flip_on_load);

    int width, height, channels;
    unsigned char* image = stbi_load(image_path.c_str(), &width, &height, &channels, desired_channels);

    if(!image){
        throw graphics_error{"Image with given filepath doesn't exist"};
    }

    return image_data{image, width, height, channels};
}

void meg::image_loader::free_image(meg::image_loader::image_data &data) {
    stbi_image_free(data.pixels);
    data.pixels = nullptr;
}

void meg::image_loader::free_image(unsigned char *data) {
    stbi_image_free(data);
}
