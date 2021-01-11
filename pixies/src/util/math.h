//
// Created by kookie on 03. 01. 2021..
//

#ifndef PIXIES_MATH_H
#define PIXIES_MATH_H

namespace kki
{
    template <typename T>
    bool inRange(const T& min, const T& max, const T& value){
        return min <= value && value <= max;
    }
}


#endif //PIXIES_MATH_H
