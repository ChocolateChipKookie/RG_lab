//
// Created by kookie on 28/03/2020.
//

#include "framebuffer.h"
#include "util/graphics_error.h"
#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <vector>

meg::framebuffer::framebuffer(const std::shared_ptr<texture>& texture) : texture_(texture), rbo_{0}, id_{0}
{
    glGenFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);

    //Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->get_id(), 0);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texture->get_width(), texture_->get_height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw meg::graphics_error{"Framebuffer is not complete!"};
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

meg::framebuffer::~framebuffer()
{
    glDeleteFramebuffers(1, &id_);
}

void meg::framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

void meg::framebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void meg::framebuffer::save_to_file(const std::string& name)
{
    auto pixels = new unsigned char[texture_->get_width() * texture_->get_height() * 3];

    /// READ THE CONTENT FROM THE FBO
    bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0, 0, texture_->get_width(), texture_->get_height(), GL_RGB, GL_UNSIGNED_BYTE, pixels);
    unbind();

    stbi_flip_vertically_on_write(1);
    stbi_write_png(name.c_str(), texture_->get_width(), texture_->get_height(), 3, pixels, texture_->get_width() * 3);

    delete[]pixels;
}




