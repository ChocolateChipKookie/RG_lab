//
// Created by kookie on 02/04/2020.
//

#ifndef MEEGERENGEN_IMAGE_LOADER_H
#define MEEGERENGEN_IMAGE_LOADER_H

#include <string>

namespace meg{
    class image_loader {
    public:
        struct image_data{
            unsigned char* pixels;
            int width, height, channels;
        };

        static image_data load_image(const std::string& image_path, bool flip_on_load = true, int desired_channels = 0);
        static void free_image(image_data& data);
        static void free_image(unsigned char* data);
    };
}




#endif //MEEGERENGEN_IMAGE_LOADER_H
