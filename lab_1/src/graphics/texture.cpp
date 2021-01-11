//
// Created by kookie on 28/03/2020.
//

#include "texture.h"
#include "util/graphics_error.h"
#include <glad/glad.h>
#include <utility>
#include "util/image_loader.h"

meg::texture::texture(const std::string& filepath, bool delete_buffer)
    :   id_{0}, width_{0}, height_{0}, channels_{0}, filename_{filepath}
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    //Set wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    auto image = meg::image_loader::load_image(filepath, true, 4);
    local_buffer_ = image.pixels;

    if (local_buffer_)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_);
        glGenerateMipmap(GL_TEXTURE_2D);
        width_ = image.width;
        height_ = image.height;
        channels_ = image.channels;
    }
    else
    {
        throw graphics_error{ "Failed to load texture" };
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    if(delete_buffer){
        meg::image_loader::free_image(image);
        local_buffer_ = nullptr;
    }
}

meg::texture::texture(const unsigned char *data, int width, int height, int channels) : id_(0), width_(width), height_(height), channels_(channels) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    //Set wrapping to repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    if(channels != 3){
        throw graphics_error{"Number of channels not supported!"};
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}



meg::texture::texture(int width, int height)
    :   id_{0}, width_{width}, height_{height}, channels_{3}, filename_{}
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}


meg::texture::~texture()
{
    glDeleteTextures(1, &id_);
}

void meg::texture::generate_mip_map() const
{
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
}

void meg::texture::bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id_);
}
