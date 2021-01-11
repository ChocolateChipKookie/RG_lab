//
// Created by kookie on 28/03/2020.
//

#ifndef MEEGERENGEN_TYPES_H
#define MEEGERENGEN_TYPES_H

#include <memory>

namespace util
{
    template <typename T>
    using ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    inline ref<T> make_ref(Args&&... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}


#endif //MEEGERENGEN_TYPES_H
