//
// Created by kookie on 18. 10. 2020..
//

#ifndef LAB_1_LINE_STRIP_H
#define LAB_1_LINE_STRIP_H


#include <vector>
#include <glm/vec3.hpp>
#include "graphics/shader.h"

class line_strip {
public:
    explicit line_strip(std::vector<glm::vec3> points) : points_{std::move(points)}{
        setup();
    };

    void draw(meg::shader& shader);
    void apply_matrix(const glm::mat4& mat);

private:
    void setup();

    unsigned int VAO{}, VBO{};
    std::vector<glm::vec3> points_;
};


#endif //LAB_1_LINE_STRIP_H
