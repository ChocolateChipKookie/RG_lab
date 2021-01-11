//
// Created by kookie on 27/03/2020.
//

#ifndef MEEGERENGEN_GRAPHICS_ERROR_H
#define MEEGERENGEN_GRAPHICS_ERROR_H

#include <stdexcept>

namespace meg
{
    class graphics_error final : public std::runtime_error
    {
    public:
        graphics_error(const std::string& message) : runtime_error(message) {};
        graphics_error(const char* message) : runtime_error(message) {};
    };
}


#endif //MEEGERENGEN_GRAPHICS_ERROR_H
