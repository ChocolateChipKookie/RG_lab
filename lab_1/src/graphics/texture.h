//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_TEXTURE_H
#define MEEGERENGEN_TEXTURE_H

#include <string>

namespace meg{
    class texture {
    public:
        explicit texture(const std::string& filepath, bool delete_buffer = true);
        explicit texture(const unsigned char* data, int width, int height, int channels);
        texture(int width, int height);
        ~texture();

        void bind(unsigned int slot = 0) const;
        void generate_mip_map() const;

        unsigned get_id() const {return id_;};
        int get_width() const {return width_;};
        int get_height() const{return height_;};
        int get_channels() const{return channels_;};

    private:
        unsigned id_;
        int width_, height_, channels_;
        unsigned char* local_buffer_{nullptr};
        std::string filename_;
    };
}




#endif //MEEGERENGEN_TEXTURE_H
