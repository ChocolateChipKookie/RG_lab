//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_FRAMEBUFFER_H
#define MEEGERENGEN_FRAMEBUFFER_H

#include <memory>
#include "texture.h"

namespace meg
{
    class framebuffer
    {
        unsigned id_;
        unsigned rbo_;
        std::shared_ptr<texture> texture_;

    public:
        explicit framebuffer(const std::shared_ptr<texture>& texture);
        ~framebuffer();

        void bind();
        void unbind();

        //Supports only 3 channel .png format
        void save_to_file(const std::string& name);

        [[nodiscard]] unsigned get_id() const {return id_;};
        [[nodiscard]] std::shared_ptr<texture> get_texture() const{return texture_;};
    };
}


#endif //MEEGERENGEN_FRAMEBUFFER_H
